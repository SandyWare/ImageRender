#ifndef SCENE_H
#define SCENE_H
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    Scene(QGraphicsScene* parent = 0);
    ~Scene();
  // void focusInEvent(QFocusEvent *event);
};

#endif // SCENE_H
