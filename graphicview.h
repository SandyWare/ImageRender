#ifndef MYGRAPHICVIEW_H
#define MYGRAPHICVIEW_H
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsItemGroup>
#include <QResizeEvent>
#include <QWheelEvent>
#include "scene.h"


class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(Scene *scene, QWidget *parent = 0);
    ~GraphicsView();
    QRectF getVisibleRegion();
protected:
    void wheelEvent(QWheelEvent*);
private:
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
//    void scale(qreal scaleFactor);
private:
    int scaleX;
    QGraphicsScene* _scene;
    bool _pan;
    int _panStartX, _panStartY;
signals:
    void sendMousePoint(QPointF point);
    void sendLevel(short);
    //void renderImageTile();
};

#endif // MYGRAPHICVIEW_H
