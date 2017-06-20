#include "graphicview.h"
#include <QDebug>
#include <QRectF>
#include <QScrollBar>
#include "globaldef.h"


GraphicsView::GraphicsView(Scene *scene, QWidget* parent)
    : QGraphicsView(scene, parent),_scene(scene),  scaleX(0),_pan(false), _panStartX(0), _panStartY(0)
{
    setMouseTracking(true);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    setDragMode(QGraphicsView::ScrollHandDrag);
    setCacheMode(QGraphicsView::CacheBackground);


    setRenderHint(QPainter::Antialiasing, false);
    //setDragMode(QGraphicsView::RubberBandDrag);
    setOptimizationFlags(QGraphicsView::DontSavePainterState);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    //setAlignment(Qt::AlignVCenter);

    horizontalScrollBar()->setRange(_scene->sceneRect().left()*scaleX * 0.9, _scene->sceneRect().right()*scaleX * 0.9);
    verticalScrollBar()->setRange(_scene->sceneRect().left()* scaleX  * 0.9, _scene->sceneRect().right()*scaleX  * 0.9);
}


void GraphicsView::wheelEvent(QWheelEvent * event)
{
    // Typical Calculations (Ref Qt Doc)
    //const int degrees = event->delta() / 8;
    int angle = event->angleDelta().y(); // angle will tell you zoom in or out.
    //_steps = degrees / 15;
    double scaleFactor = 1.15; //How fast we zoom
    //const qreal minFactor = 1.0;

    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    //if(steps > 0)
    if (angle > 0)
    {
        qDebug()<<"Zoom in";
        this->scale(scaleFactor, scaleFactor);
        emit sendLevel(0);
    }
    else
    {
        qDebug()<<"Zoom out";
        this->scale(1.0 / scaleFactor, 1.0 / scaleFactor);
        emit sendLevel(1);
    }
}

//release panning
void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        _pan = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
        return;
    }
    //event->ignore();
    QGraphicsView::mouseReleaseEvent(event);
}

//start panning
void GraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        _pan = true;
        _panStartX = event->x();
        _panStartY = event->y();
        setCursor(Qt::ClosedHandCursor);
        event->accept();
        return;
    }
    //event->ignore();
    QGraphicsView::mousePressEvent(event);
}


//QRectF GraphicsView::getVisibleRegion()
//{
//    QPointF topLeft = mapToScene (0, 0);
//    QPointF bottomRight = mapToScene (this->width(), this->height());
//    return QRectF (topLeft, bottomRight);
//}

QRectF GraphicsView::getVisibleRegion()
{
    QPointF tl(horizontalScrollBar()->value(), verticalScrollBar()->value());
    QPointF br = tl + viewport()->rect().bottomRight();
    QMatrix mat = matrix().inverted();
    return mat.mapRect(QRectF(tl,br));
}

//move panning
void GraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    //raise mouse movement signal start
    QPointF movment = mapToScene (event->pos());
    emit sendMousePoint(movment);

    //sw temp code start
    //QRectF r1 = getVisibleRegion();
    //qDebug()<<"GraphicsView rec: "<<r1.x() <<","<<r1.y()<<","<<r1.width()<<","<<r1.height();
    //
    //sw temp code end

    if (_pan)
    {
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - (event->x() - _panStartX));
        verticalScrollBar()->setValue(verticalScrollBar()->value() - (event->y() - _panStartY));
        _panStartX = event->x();
        _panStartY = event->y();
        //qDebug()<<"GraphicsView mouseMoveEvent";
        //emit renderImageTile();
        event->accept();
        //   qDebug() <<"Mouse X" << QString::number(event->pos().x()) << " Y:" << QString::number(event->pos().y());
        return;
    }
    //temp raise mouse movement signal end
    event->ignore();
    QGraphicsView::mousePressEvent(event);
}


//set zoom in & out max level.
//void GraphicsView::scale(qreal scaleFactor)
//{
//    QRectF r(0, 0, 1, 1); // A reference
//    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(r).width(); // absolute zoom factor
//    double minval1 = (double) WindowWidth/(IMAGEWIDTH + TileWidth);
//    double minval2 = (double) WindowHeight/(IMAGEHEIGHT + (TileHeight + TileHeight)); //height of statusbar, menubar, title bar, etc.
//    double minval = (minval1 < minval2) ? minval1 : minval2;

//    //qDebug() <<"SW minval1 :"<<minval1 <<" minval2:"<<minval2 <<" Final:"<<minval<<" factor"<<factor;

//    if ( factor > 7) { // Check zoom in limit
//        //qDebug() <<"Zoom in factor : " <<factor << " pos_y : " <<pos_y <<" scaleFactor:" <<scaleFactor;
//        return;
//    }
//    //  if(factor < minval1 && factor < minval2)
//    if(factor < minval)
//    {
//        //qDebug() <<"Zoom out factor : " <<factor << " pos_y : " <<pos_y <<" scaleFactor:" <<scaleFactor;
//        this->horizontalScrollBar()->setValue(0);
//        return;
//    }
//    QGraphicsView::scale(scaleFactor, scaleFactor);
//}

GraphicsView::~GraphicsView()
{
}
