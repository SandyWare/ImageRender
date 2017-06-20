#include "mysquare.h"
#include <QtWidgets>
#include <QDebug>
#include "OSTReadHelper.h"
#include "globaldef.h"


mySquare::mySquare(QObject *parent, int width, int height, int level, int row, int col, double minlod, \
                   double maxlod,  QImage *image) :
    QObject(parent), _width(width),_height(height),_level(level),_row(row),_col(col),
    _minlod(minlod),_maxlod(maxlod), _image(image)

{
    _left = -1;
    _top  =-1;
    _imageAttached = false;
    //setFlag(QGraphicsItem::ItemIgnoresTransformations);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setFlag(QGraphicsItem::ItemIsPanel);
    setZValue(100);
    //setFlag(QGraphicsItem::DeviceCoordinateCache);
}

QRectF mySquare::boundingRect() const
{
    return QRectF(0, 0, _width,_height);
}

//check is image assigned to object.
bool mySquare::IsImageAttached()
{
    return _imageAttached;
}

//set Image Attached /detached to object.
void mySquare::setImageAttached(bool assign)
{
    _imageAttached = assign;
}


bool mySquare::setImageBuff(unsigned char *buffer, long imageWidth, long imageHeight )
{
    size_t  size = imageWidth * imageHeight * 3;
    int  bytesPerLine = size / imageHeight;

    _image = new  QImage(reinterpret_cast<const uchar *>(buffer),imageWidth, imageHeight, bytesPerLine, QImage::Format_RGB888);
    if(_image->isNull())
    {
        qDebug() << "Error!!! failed to create a image!";
        return false;
    }
   // QString tempName = QString("SW_%1.bmp").arg(tempCout);
    //m_imag->save(tempName);
    return true;
}

void mySquare::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    const qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
    qDebug()<<"Value of lod :"<<lod;
   if((lod>_minlod) && (lod<=_maxlod))
    {
        //
    }
    else
    {
        //QRectF rect(0,0,_width,_height);
        //painter->drawRect(rect);
        return;
    }
    //qDebug()<<"Inside paint:l,r,c" <<_level<< ","<<_row<<","<<_col;
    if((_level==0) && (_row==0) && (_col==0))
    {
        qDebug()<<" Level: "<<_level<<" lod:"<<lod;
    }
    qDebug()<<"Value of lod :"<<lod;

    QRectF rect(0,0,_width,_height);
    if(_image != NULL)
        painter->drawImage(rect, *_image);

    QString text1 = QString("I am at level : %1 , Row: %2, Col : %3").arg(_level).arg(_row).arg(_col);
    //qDebug()<<text1;
    painter->drawText( QRectF(40, 40, _width -20, _height -20),text1);
}


mySquare::~mySquare()
{
    if(_image != NULL)
    {
        delete _image;
        _image = NULL;
    }
}

void mySquare::setImagPos(long left, long  top)
{
    _left = left;
    _top  = top;
}

void mySquare::getImagPos(long & left,long  & top, int & row, int & column)
{
    left = _left;
    top = _top;
    row = _row;
    column = _col;
}

void mySquare::releaseImage()
{
    if(_image != NULL)
    {
        delete _image;
        _image = NULL;
        //_image = Q_NULLPTR;
    }
}


void mySquare::loadImageData(int level,int row, int coloumn)
{
    unsigned char* ImgBuffer =  NULL;
    OSTReader::COSTReadHelper ostReaderHelper;
    int bufWidth = 0;
    int bufHeight = 0;
    int imageType = 0;
    int offsetX = coloumn * TileWidth;
    int offsetY = row * TileHeight;

    ostReaderHelper.GetOSTROI(0,level, offsetX, offsetY, TileWidth, TileHeight, ImgBuffer, \
                             bufWidth, bufHeight, imageType);
    if(bufWidth != 256 || bufHeight != 256)
    {
        setImageBuff(ImgBuffer,bufWidth,bufHeight);
        //render image.
        //square->update();
        //        qDebug()<<"Error: TileWidth Or TileHeight are small in size Width :"<<bufWidth <<" : Height"<< bufHeight;
    }
    else
    {
        setImageBuff(ImgBuffer, TileWidth, TileHeight);
        //render image.
        //square->update();
    }
}
