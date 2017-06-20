#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLayout>
#include <QImageReader>
#include <QBuffer>
#include <fstream>
#include <QDir>
#include <QDebug>
#include <QScreen>
#include <QDesktopWidget>
#include <algorithm>
#include <iterator>
#include "scene.h"
#include "globaldef.h"

int WindowWidth;
int WindowHeight;
long IMAGEHEIGHT;
long IMAGEWIDTH;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), _levels(-1) , _bestZoomLevel(0), _curZoomLevel(-1) , _imag(Q_NULLPTR)
{
    ui->setupUi(this);
    setWindowTitle(tr("QT ImageViwer"));
    setWindowIcon(QIcon(":FLViewer.ico"));
    Qt::WindowFlags flags = (Qt::Window );
    setWindowFlags(flags);
    _imageLevel.high = -1, _imageLevel.low = -1;

    //add Widget to centeral.
    createCentralWidget();

    //set window size as per desktop
    centerAndResize();

    //add graphic view
    addGPView();

    //open Image
    openImage();

    //find start position
    //long startPos = findStartPos();

    loadMetaDataAsperLevel(-1);

    //add data to Status Bar
    addStatusBar();
}

void MainWindow::createCentralWidget()
{
    _scene = new Scene();
    _myGraphicsView =  new GraphicsView(_scene);

    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(_myGraphicsView, 1);
    this->centralWidget()->setLayout(layout);

    //connect GraphicsView signal to main window status bar slot.
    QWidget::connect (_myGraphicsView, SIGNAL(sendMousePoint(QPointF)),this, SLOT(setMousePoint(QPointF)));

    //connect GraphicsView send zoom levels.
    QWidget::connect (_myGraphicsView, SIGNAL(sendLevel(short)), this, SLOT(loadMetaDataAsperLevel(short)));

    //connect GraphicsView to render image tiles.
    //QWidget::connect(_myGraphicsView, SIGNAL(renderImageTile()), this, SLOT(loadTileImage()));
}

void MainWindow::addStatusBar()
{
    QString imgHighWidth = QString("Image Size(%1 , %2)").arg(IMAGEHEIGHT).arg(IMAGEWIDTH);

    _imagePos = new QLabel("image Position");
    //_imagePos->setAlignment(Qt::AlignRight);
    _imagePos->setAlignment(Qt::AlignJustify);
    //  _imagePos->setMinimumSize(_imagePos->sizeHint());
    _imagePos->setMinimumSize(QSize(10,5));

    _imagePos->setText("0,0");

    _imageSize = new QLabel("image size");
    _imageSize->setAlignment(Qt::AlignJustify);
    _imageSize->setMinimumSize(QSize(10,5));

    _imageSize->setText(imgHighWidth);
    statusBar()->addWidget(_imagePos, 1);
    statusBar()->addWidget(_imageSize, 2);
}

MainWindow::~MainWindow()
{
    std::map<unsigned  long, mySquare*>::iterator it = _ImagesList.begin();
    for(; it != _ImagesList.end(); it++)
    {
        mySquare *square = (*it).second;
        //delete square;

    }
    delete _scene;
    delete _myGraphicsView;
    delete ui;
    delete _imagePos;
    delete _imageSize;
}

void MainWindow::openImage()
{
    QString imgfileName = QDir::homePath();
    //imgfileName += "/task/Slide1_170515_150700.otif";
    imgfileName += "/task/Slide2.otif";
    //qDebug()<<"Img path:"<<imgfileName;
    _OSTReadHelper.OpenImage((char*)imgfileName.toLatin1().data());
    //get image height & width.
    IMAGEHEIGHT =   _OSTReadHelper.GetHeight(0,0);
    IMAGEWIDTH =   _OSTReadHelper.GetWidth(0,0);

    _levels= _OSTReadHelper.GetLevels();

    //find best sutiable zoom level
    _bestZoomLevel = findZoomLevel();
    _curZoomLevel = _bestZoomLevel;

    _imageLevel.low = 0;
    for(int level=0; level<_levels; level++)
    {
        int imageLevelWidth = _OSTReadHelper.GetWidth(0,level);
        int imageLevelHeight = _OSTReadHelper.GetWidth(0,level);
        if( imageLevelWidth > TileWidth && imageLevelHeight > TileHeight)
        {
            _imageLevel.high = level;
        }
    }
}


void MainWindow::loadTileImage()
{
    double minlod = 0.5;
    double maxlod = 1.0;
    _hashKeyList.clear();
    QRectF rec = _myGraphicsView->getVisibleRegion();
    QList<QGraphicsItem *> list = _scene->items(rec,Qt::IntersectsItemBoundingRect);
    QList<QGraphicsItem *>::iterator it = list.begin();
    for(; it != list.end(); it++)
    {
        QGraphicsItem *myItem = *it;
        QPointF pt = myItem->scenePos();
        unsigned long hashKey = getHaskey(_curZoomLevel, pt.x(), pt.y());
        _hashKeyList.push_back(hashKey);
    }

    //load image data
    std::vector <unsigned long>::iterator ithash = _hashKeyList.begin();
    for(; ithash != _hashKeyList.end(); ithash++)
    {
        unsigned long squareHashKey = *ithash;
        mySquare *square =  _ImagesList[squareHashKey];
        if(square != NULL)
        {
            bool isImageAttached = square->IsImageAttached();
            //If image is not attached, then only load image.
            if(isImageAttached == false)
            {
                long left = -1, top = -1;
                int row = -1, colm = -1;
                square->getImagPos(left, top, row, colm);
                //qDebug()<<"left :"<< left <<"top :"<<top <<"row : "<<row <<"colm :"<<colm;
                loadImageData(square, _curZoomLevel, row, colm);
                square->setImageAttached(true);
            }
        }
        else
        {
            qDebug()<<"Error!!! Failed to get square object";
        }
    }

    //remove nonfocus images
    if ( _lastImageList.size() >0)
    {
        removeImages(_hashKeyList, _lastImageList);
    }
    //qDebug()<< hashKey <<" x: "<<pt.x() <<" y: "<<pt.y();
    //qDebug()<<"MainWindow pos x: "<<pt.x() <<" y: "<<pt.y();
    //qDebug()<<"MainWindow rec: "<<rec.x() <<","<<rec.y()<<","<<rec.width()<<","<<rec.height();
    // }
    //qDebug()<<"MainWindow rec: "<<rec.x() <<","<<rec.y()<<","<<rec.width()<<","<<rec.height();
    //qDebug()<<"SW itemCount : " <<list.count();
    _lastImageList = _hashKeyList;
}

void MainWindow::removeImages(std::vector <unsigned long> currList, std::vector <unsigned long> prevList )
{
    std::vector <unsigned long> resList;

    std::sort(currList.begin(), currList.end());
    std::sort(prevList.begin(), prevList.end());

    std::vector<unsigned long>::iterator currIt = currList.begin();
    std::vector<unsigned long>::iterator prevIt = prevList.begin();
    std::set_difference(prevList.begin(), prevList.end(), currList.begin(), currList.end(), std::back_inserter(resList));

    std::vector<unsigned long>::iterator it = resList.begin();
    for (; it != resList.end(); it++)
    {
        unsigned long haskey =  *it;
        //qDebug() <<"haskey :"<<haskey;
        mySquare *square =  _ImagesList[haskey];
        if(square != NULL)
        {
            bool isImageAttached =  square->IsImageAttached();
            if(isImageAttached == true)
            {
                square->releaseImage();
                square->setImageAttached(false);
                square->update();
                square->show();
            }
        }
    }
}


void MainWindow::loadMetaDataAsperLevel(short zoomIn)
{
    //find start position
    long startPos = findStartPos();
    bool levelChange = false;

    if( (_curZoomLevel + 1 <= _imageLevel.high ) && (zoomIn == 1) )
    {
        _curZoomLevel++;
        levelChange = true;
        _scene->clear();
        _ImagesList.clear();
        _lastImageList.clear();
    }

    if( (_curZoomLevel - 1 >= _imageLevel.low) && (zoomIn == 0) ){
        _curZoomLevel--;
        levelChange = true;
        _scene->clear();
        _ImagesList.clear();
        _lastImageList.clear();
    }
    if(levelChange == false && (zoomIn != -1))  //if level not updated return;
    {
        return;
    }

    int noCol = -1;
    int noRow = -1;
    int imageLevelWidth = _OSTReadHelper.GetWidth(0,_curZoomLevel);
    int imageLevelHeight = _OSTReadHelper.GetWidth(0,_curZoomLevel);
    if(imageLevelWidth % TileWidth == 0 )
    {
        noCol = imageLevelWidth / TileWidth;
        noCol--;
    }
    else
    {
        noCol = imageLevelWidth / TileWidth;
    }
    if(imageLevelHeight % TileHeight == 0 )
    {
        noRow = imageLevelHeight / TileHeight;
        noRow--;
    }
    else
    {
        noRow = imageLevelHeight / TileHeight;
    }

    qDebug() <<"ZoomLevel ->" << _curZoomLevel;
    int count = 0;
    long left = 0;
    long top = 0;
    //set out paramter for OSTReadHelper.
    //unsigned char* ImgBuffer =  NULL;
    int bufWidth =0;
    int bufHeight =0;
    int imageType =0;

    int width = TileWidth;
    int height = TileHeight;

    double minlod= 0.5;
    double maxlod= 1.0;
    //minlod/= (2.0 * level;
    //maxlod/=2.0  * level;
    //    for(int level=0;level<_levels;level++)
    //    {
    left= startPos;
    top= 0;

    QImage *blankImage = Q_NULLPTR;
    for ( int i= 0; i<=noRow; i++ ) {
        for ( int j= 0; j<= noCol ; j++ ) {
            _square = new mySquare(0,width, height, _curZoomLevel,i,j, \
                                   minlod,maxlod, blankImage);
            _square->setPos(QPointF(left, top));
            //added to support easy debug.
            _square->setImagPos(left, top);
            //temp code start
            _scene->addItem(_square);

            // qDebug()<<"left: "<<left <<" top:"<<top;
            unsigned long hashKey = getHaskey(_curZoomLevel, left, top);
            //qDebug()<< hashKey;
            _ImagesList[hashKey] = _square;
            count++;
            left += width;
        }
        left = startPos;
        top += height;
    }

    width*= 2;
    height*=2;
    // minlod/=2.0;
    //  maxlod/=2.0;
    //}
}


void MainWindow::loadImageData(mySquare *square, int level,int row, int coloumn)
{
    unsigned char* ImgBuffer =  NULL;
    int bufWidth = 0;
    int bufHeight = 0;
    int imageType = 0;
    int offsetX = coloumn * TileWidth;
    int offsetY = row * TileHeight;

    _OSTReadHelper.GetOSTROI(0,level, offsetX, offsetY, TileWidth, TileHeight, ImgBuffer, \
                             bufWidth, bufHeight, imageType);
    if(bufWidth != 256 || bufHeight != 256)
    {
        square->setImageBuff(ImgBuffer,bufWidth,bufHeight);
        //render image.
        square->update();
        //        qDebug()<<"Error: TileWidth Or TileHeight are small in size Width :"<<bufWidth <<" : Height"<< bufHeight;
    }
    else
    {
        square->setImageBuff(ImgBuffer, TileWidth, TileHeight);
        //render image.
        square->update();
    }
}


unsigned long MainWindow::getHaskey(int zoomLevel, long left, long top)
{
    QString haskeyStr = "";
    for(int k=0; k <=zoomLevel; k++ )
    {
        haskeyStr += "1";
    }
    haskeyStr += QString::number(left);
    haskeyStr += QString::number(top);
    //qDebug() <<"hashKey : "<< haskeyStr;
    unsigned long hashKey = haskeyStr.toULong();
    return hashKey;
}


bool MainWindow::loadImageBuff(unsigned char *buffer, long imageWidth, long imageHeight )
{
    //size_t  size = strlen((char*)buffer);
    size_t  size = imageWidth * imageHeight * 3;
    //QByteArray array((const char *)buffer, size);
    //int imgSize = array.size() ;
    //int  bytesPerLine = imgSize / imageHeight;
    int  bytesPerLine = size / imageHeight;

    //QImage img1 (reinterpret_cast<const uchar *>(buffer),TileWidth, TileHeight, bytesPerLine, QImage::Format_RGB888);
    _imag = new  QImage(reinterpret_cast<const uchar *>(buffer),imageWidth, imageHeight, bytesPerLine, QImage::Format_RGB888);
    if(_imag->isNull())
    {
        qDebug() << "Error!!! failed to create a image!";
        return false;
    }
   // QString tempName = QString("SW_%1.bmp").arg(tempCout);
    //_imag->save(tempName);
    return true;
}


void MainWindow::addGPView()
{
    _myGraphicsView->setScene(_scene);
    //_myGraphicsView->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    _myGraphicsView->setAlignment(Qt::AlignLeading);

    _scene->clear();
    QSize newsize = this->size();
    //qreal w = newsize.width()*.99;
    //qreal h = newsize.height()*.99;
    setCentralWidget(_myGraphicsView);
}

//set status bar with value of graphics.
void MainWindow::setMousePoint(QPointF point)
{
    //qDebug()<<"SW :->" <<point.x() << point.y();
    QString MousePosVal = "   Mouse Pos:(" + QString::number(point.x()) + " , " + QString::number(point.y()) + ")";
    _imagePos->setText(MousePosVal);
}

void MainWindow::centerAndResize()
{
    // get the dimension available on this screen
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
    int height = screenGeometry.height();
    int width = screenGeometry.width();

    qDebug() << "Available dimensions " << width << "x" << height;
    width *= 0.98; // 98% of the screen size
    height *= 0.91; // 91% of the screen size
    WindowWidth = width;
    WindowHeight = height;

   // qDebug() << "Computed dimensions " << width << "x" << height;
    QSize newSize( width, height );
    setGeometry(
                QStyle::alignedRect(
                    Qt::LeftToRight,
                    Qt::AlignCenter,
                    newSize,
                    qApp->desktop()->availableGeometry()
                    )
                );
}

long MainWindow::findStartPos()
{
    long startPos = 0;
    if (WindowWidth > IMAGEWIDTH)
    {
        long remSpace = WindowWidth - IMAGEWIDTH;
        startPos = remSpace / 2;
    }
    return startPos;
}


int MainWindow::findZoomLevel()
{
    int bestDist = 0;
    long bestLevel = -1;
    const int imgSize =  WindowWidth + WindowHeight;

    for(int level=0; level<_levels; level++)
    {
        int width  = _OSTReadHelper.GetWidth(0, level);
        int height = _OSTReadHelper.GetHeight(0, level);
        int currentDist =   imgSize - (width + height);
        currentDist =    abs(currentDist);
        if(bestDist == 0) {
            bestDist = currentDist;
            bestLevel = level;
        }
        if (currentDist < bestDist ) {
            bestDist = currentDist;
            bestLevel = level;
        }
    }
    //qDebug()<<"Best level is: " << bestLevel <<" bestDist :  " <<bestDist;
    return bestLevel;
}


void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
   // loadTileImage();
}

void MainWindow::paintEvent(QPaintEvent *pe){
  // qDebug() << Q_FUNC_INFO;
   QMainWindow::paintEvent(pe);
   loadTileImage();
 }

//void MainWindow::ReleaselastLevelData()
//{
//    std::map<unsigned long, mySquare*>::iterator it = _ImagesList.begin();
//    for(;it != _ImagesList.end(); it++)
//    {
//        mySquare* square = it->second;
//        if(square != NULL)
//            delete square;
//        square =  NULL;
//    }
//}

