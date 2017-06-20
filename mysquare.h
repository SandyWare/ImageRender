#ifndef MYSQUARE_H
#define MYSQUARE_H
#include <QPainter>
#include <QGraphicsItem>
#include <QDebug>
#include <QResizeEvent>


//class mySquare : public QGraphicsItem
class mySquare : public QObject, public QGraphicsRectItem
{
        Q_OBJECT
public:
    explicit mySquare(QObject *parent = 0, int width = 0 , int height =0 , int level =0 , int row =0 , int col =0, \
             double minlod =0, double maxlod =0, QImage *image = Q_NULLPTR);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setImagPos(long left,long  top);
    void getImagPos(long & left,long  & top, int & row, int & column);
    bool setImageBuff(unsigned char *buffer, long imageWidth, long imageHeight);
    void releaseImage();
    ~mySquare();
protected:
    //    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    //    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void loadImageData(int level,int row, int coloumn);

protected:
    int _width,_height;
    QBrush m_brush;
    int _level,_row,_col, _left, _top;
    double _minlod,_maxlod;
    QImage *_image;
    bool _imageAttached;
    //QString _imgfileName;
    //    void resizeEvent(QResizeEvent *event);
signals:
    //void getImageTile(int level, int coloumn, int row, QImage *imag);
public:
    bool IsImageAttached();
    void setImageAttached(bool status);
};

#endif // MYSQUARE_H
