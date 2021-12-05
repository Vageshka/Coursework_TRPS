#include "diagramscene.h"

#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <delegatefactory.h>

DiagramScene::DiagramScene(QObject *parent) : QGraphicsScene(parent),
                arrowType_(Arrow::StraightArrow), itemType_(DiagramItem::Regular)
{
    state = MoveItem;
    paper = new Paper();
    setSceneRect(-6000, -6000, 19000, 19000);
    addItem(paper);
    setBackgroundBrush(QBrush(Qt::lightGray));
    headType = Arrow::acute;
    QFont f = font();
    f.setPixelSize(14);
    setFont(f);

    dashLine = new QGraphicsLineItem(QLineF(0, 0, 0, 0));
    dashLine->setPen(QPen(QBrush(Qt::black), 2, Qt::DashLine));
    dashLine->setZValue(10000);
    addItem(dashLine);
    dashLine->setVisible(false);

    selectingRect = new QGraphicsRectItem(0, 0, 0, 0);
    selectingRect->setPen(QPen(QColor(134, 206, 250), 1));
    selectingRect->setBrush(QColor(134, 206, 250, 150));
    selectingRect->setZValue(10000);
    addItem(selectingRect);
    selectingRect->setVisible(false);
}

void DiagramScene::updateArrowsItems()
{
    QList<QGraphicsItem *> all = items();
    foreach( QGraphicsItem *item, all ) {
        auto ar = dynamic_cast<Arrow*>(item);
        if(ar){
            ar->updateItems();
        }
    }
}

QList<DiagramItem *> DiagramScene::selectedDiagramItems() const
{
    QList<DiagramItem *> items;
    foreach(auto i, selectedItems()) {
        if(auto x = dynamic_cast<DiagramItem *>(i)) {
            items.push_back(x);
        }
    }

    return items;
}

QList<Arrow *> DiagramScene::selectedArrows() const
{
    QList<Arrow *> arrows;
    foreach(auto i, selectedItems()) {
        if(auto x = dynamic_cast<Arrow *>(i)) {
            arrows.push_back(x);
        }
    }

    return arrows;
}

QList<DiagramItem *> DiagramScene::diagramItems() const
{
    QList<DiagramItem *> diagramItems;
    foreach(auto i, items()) {
        if(auto x = dynamic_cast<DiagramItem *>(i)) {
            diagramItems.push_back(x);
        }
    }

    return diagramItems;
}

QList<Arrow *> DiagramScene::arrows() const
{
    QList<Arrow *> arrows;
    foreach(auto i, items()) {
        if(auto x = dynamic_cast<Arrow *>(i)) {
            arrows.push_back(x);
        }
    }

    return arrows;
}

bool DiagramScene::isClear()
{
    bool flag = true;
    if(items().size() != 3) flag = false;
    if(paper->size() != Paper::A4) flag = false;
    if(paper->orientation() != Paper::Portrait) flag = false;
    if(paper->brush().color() != Qt::white) flag = false;
    return flag;
}

void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    switch(state){
    case MoveItem: {
        origPoint = event->scenePos();
        selectingRect->setRect(0,0,0,0);
        selectingRect->setVisible(true);
        QGraphicsScene::mousePressEvent(event);
        break;
    }
    case AddItem: {
        DelegateFactory f;
        qDebug() << f.setItemFactory(itemType_) << itemType_;
        if(!f.setItemFactory(itemType_)) {
            return;
        }
        auto new_item = f.createItem(itemSubtype_);
        new_item->setPos(event->scenePos());
        new_item->setFont(font());
        addItem(new_item);
        break;
    }
    case AddArrow: {
        dashLine->setLine(QLineF(event->scenePos(), event->scenePos()));
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

    if(state == MoveItem && !mouseGrabberItem()) {
        float top = qMin(origPoint.y(), event->scenePos().y());
        float left = qMin(origPoint.x(), event->scenePos().x());
        selectingRect->setPos(left, top);
        selectingRect->setRect(0, 0, qAbs(event->scenePos().x() - origPoint.x()),
                                   qAbs(event->scenePos().y() - origPoint.y()));
        clearSelection();
        for(auto i : selectingRect->collidingItems()) {
            i->setSelected(true);
        }
    }

    QGraphicsScene::mouseMoveEvent(event);
}

void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(state == AddArrow) {
        dashLine->setVisible(false);

        QTransform transform(0,0,0,0,0,0);
        DiagramItem *last = dynamic_cast<DiagramItem*>(itemAt(event->scenePos(), transform));

        if((dashLine->line().length() > 20) && (!firstItem || firstItem != last)){
            DelegateFactory f;
            qDebug() << f.setArrowFactory(arrowType_) << arrowType_;
            if(!f.setArrowFactory(arrowType_)) {
                return;
            }
            auto new_arrow = f.createArrow(dashLine->line(), firstItem, last);
            new_arrow->setArrowhead(headType);
            qDebug() << new_arrow->arrowType();

            addItem(new_arrow);
        }
    }

    if(state == MoveItem) {
//        for(auto i : selectingRect->collidingItems()) {
//            i->setSelected(true);
//        }
        selectingRect->setVisible(false);
    }
    QGraphicsScene::mouseReleaseEvent(event);
}

//DiagramItem *DiagramScene::newItem()
//{
//    DiagramItem *item(nullptr);
//    DiagramItem::ItemType iType = static_cast<DiagramItem::ItemType>(itemTypeFlags / 1000);
//    unsigned int iSubtype = itemTypeFlags % 1000;
//    switch (iType) {
//    case DiagramItem::Regular: {
//        item = new RegularVertex((RegularVertex::VertexType)iSubtype);
//        break;
//    }
//    case DiagramItem::Container: {
////        item;
//        break;
//    }
//    }
//    return item;
//}

//bool DiagramScene::itemTypeFlagsValid()
//{
//    unsigned int iType = itemTypeFlags / 1000;
//    if((iType < DiagramItem::begin) || (iType > DiagramItem::end)) {
//        return false;
//    }

//    switch (iType) {
//    case DiagramItem::Regular: {
//        RegularVertex::VertexType iSubtype = static_cast<RegularVertex::VertexType>(itemTypeFlags % 1000);
//        if((iSubtype < RegularVertex::begin) || (iType > RegularVertex::end)) {
//            return false;
//        }
//        break;
//    }
//    case DiagramItem::Container: {
////        item;
//        break;
//    }
//    }

//    return true;
//}
