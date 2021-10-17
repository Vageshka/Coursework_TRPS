#include "diagramscene.h"

#include <QDebug>
#include <QGraphicsSceneMouseEvent>

DiagramScene::DiagramScene(QObject *parent) : QGraphicsScene(parent)
{
    state = MoveItem;
    paper = new Paper();
    addItem(paper);
    setBackgroundBrush(QBrush(Qt::lightGray));
//    setSceneRect(paper->rect());
}

void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    switch(state){
    case MoveItem: {
        QGraphicsScene::mousePressEvent(event);
        break;
    }
    case AddItem: {
        DiagramItem *new_item = new DiagramItem();
//        qDebug() << event->scenePos();
        qDebug() << new_item->zValue();
        new_item->setPos(event->scenePos());
        addItem(new_item);
//        newVertex();
//        addItem()
        break;
    }
    case AddArrow: {
        QGraphicsScene::mousePressEvent(event);
        break;
    }
    }
}
