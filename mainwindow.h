#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <map>
#include <vector>
#include <QLabel>
#include "mysquare.h"
#include "graphicview.h"
#include "scene.h"
#include "OSTReadHelper.h"

typedef struct Imagelevel
{
    int high =-1;
    int low =-1;
} ImageLevel;

namespace Ui {
class MainWindow;
}



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void resizeEvent(QResizeEvent* event);
    //qreal QGraphicsItem::scale() const

private:
    Ui::MainWindow *ui;
    //QGraphicsScene *_scene;
    Scene *_scene;
    mySquare *_square;
    GraphicsView *_myGraphicsView;
    int _levels;
    int _bestZoomLevel;
    int _curZoomLevel;
private:
    std::map<unsigned long, mySquare*> _ImagesList;
    //QImage *m_imag;
    QImage *_imag;
    QLabel* _imagePos;
    QLabel* _imageSize;
    OSTReader::COSTReadHelper _OSTReadHelper;
    ImageLevel _imageLevel;

protected:
    void paintEvent(QPaintEvent *pe);
    void openImage();
    void addItems(long startPos);
    void addGPView();
    bool loadImageBuff(unsigned char *buffer, long imageWidth, long imageHeight);
    void addStatusBar();
    void createCentralWidget();
    void centerAndResize();
    int findZoomLevel();
    void removeImages(std::vector <unsigned long> currList, std::vector <unsigned long> prevList );
private:
    long findStartPos();
    void ScrollImages(long startPos);
    unsigned long getHaskey(int zoomLevel, long left, long top);
    std::vector <unsigned long> _hashKeyList;
    std::vector <unsigned long> _lastImageList;
    //void ReleaselastLevelData();

public slots:
    void setMousePoint (QPointF point);
    void loadMetaDataAsperLevel(short zoomIn);
    void loadTileImage();
    void loadImageData(mySquare *square, int level, int row, int coloumn);
    //void loadTileImage();
};

#endif // MAINWINDOW_H
