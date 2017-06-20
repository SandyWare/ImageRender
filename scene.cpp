#include "scene.h"
#include <QDebug>
#include "mainwindow.h"
#include "globaldef.h"


Scene::Scene(QGraphicsScene *parent)
    : QGraphicsScene(parent)
{
    setSceneRect(0,0, IMAGEHEIGHT, IMAGEWIDTH);
    //setAlignment(Qt::AlignLeading);
}

Scene::~Scene()
{
}

//void Scene::focusInEvent(QFocusEvent *event)
//{
//    //qDebug() <<"Scene::focusInEvent";
//    //QRectF rec = sceneRect();
//    //qDebug() <<"x: "<< rec.x() <<"y: "<< rec.y() <<" width : "<<rec.width() <<" height : "<<rec.height() ;

//    //QList<QGraphicsItem *> list = selectedItems();
//    //qDebug() <<"QGraphicsItem list count "<<list.count();
//}
