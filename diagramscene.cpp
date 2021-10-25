#include "diagramscene.h"

#include <QDebug>
#include <QGraphicsSceneMouseEvent>

DiagramScene::DiagramScene(QObject *parent) : QGraphicsScene(parent)
{
    state = MoveItem;
    paper = new Paper();
    addItem(paper);
    setBackgroundBrush(QBrush(Qt::lightGray));

    dashLine = new QGraphicsLineItem(QLineF(0, 0, 0, 0));
    dashLine->setPen(QPen(QBrush(Qt::black), 2, Qt::DashLine));
    addItem(dashLine);
    dashLine->setVisible(false);
}

void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    switch(state){
    case MoveItem: {
        QGraphicsScene::mousePressEvent(event);
        break;
    }
    case AddItem: {
        if(!itemTypeFlagsValid()) {
            return;
        }

        auto new_item = newItem();
        new_item->setPos(event->scenePos());
        addItem(new_item);
        break;
    }
    case AddArrow: {
        dashLine->setLine(QLineF(event->scenePos(), event->scenePos()));
        dashLine->setZValue(10000);
        dashLine->setVisible(true);
        QTransform transform(0,0,0,0,0,0);
        firstItem = dynamic_cast<DiagramItem*>(itemAt(event->scenePos(), transform));
        break;
    }
    }
}

void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(state == AddArrow) {
        QLineF l = dashLine->line();
        l.setP2(event->scenePos());
        dashLine->setLine(l);
    }

    QGraphicsScene::mouseMoveEvent(event);
}

void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(state == AddArrow) {
        dashLine->setVisible(false);

        QTransform transform(0,0,0,0,0,0);
        DiagramItem *last = dynamic_cast<DiagramItem*>(itemAt(event->scenePos(), transform));

        if(!firstItem || firstItem != last){
            Arrow *new_arrow = new Arrow(dashLine->line(), firstItem, last);

            addItem(new_arrow);
        }
    }

    QGraphicsScene::mouseReleaseEvent(event);
}

DiagramItem *DiagramScene::newItem()
{
    DiagramItem *item;
    DiagramItem::ItemType iType = static_cast<DiagramItem::ItemType>(itemTypeFlags / 1000);
    unsigned int iSubtype = itemTypeFlags % 1000;
    switch (iType) {
    case DiagramItem::Regular: {
        item = new RegularVertex((RegularVertex::VertexType)iSubtype);
        break;
    }
    case DiagramItem::Container: {
//        item;
        break;
    }
    }
    return item;
}

bool DiagramScene::itemTypeFlagsValid()
{
    unsigned int iType = itemTypeFlags / 1000;
    if((iType < DiagramItem::begin) || (iType > DiagramItem::end)) {
        return false;
    }

    switch (iType) {
    case DiagramItem::Regular: {
        RegularVertex::VertexType iSubtype = static_cast<RegularVertex::VertexType>(itemTypeFlags % 1000);
        if((iSubtype < RegularVertex::begin) || (iType > RegularVertex::end)) {
            return false;
        }
        break;
    }
    case DiagramItem::Container: {
//        item;
        break;
    }
    }

    return true;
}
