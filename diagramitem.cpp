#include "diagramitem.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>


#define DiagramItem_SIZE QSize(150, 100)
#define RESIZING_RECT_WIDTH 10
#define MINIMAL_DiagramItem_WIDTH 50
#define MINIMAL_DiagramItem_HEIGHT 100

DiagramItem::DiagramItem(QGraphicsItem* parentItem, QObject *parent) : QObject(parent), QGraphicsItem(parentItem)
{
    size = DiagramItem_SIZE;
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
}


QRectF DiagramItem::boundingRect() const
{
    int half = RESIZING_RECT_WIDTH * 0.5;
    return QRectF (QPointF(0,0), size.grownBy(QMargins(half, half, half, half)));//.marginsAdded(QMarginsF(5,5,5,5));
}

void DiagramItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::black);
    painter->setBrush(QBrush(Qt::white));
//    this->isSelected() ? painter->setBrush(Qt::red) : painter->setBrush(Qt::green);
    if(this->isSelected())
    {
        for(int i=Top; i <= BottomRight; i++){
            painter->drawRect(getResizingRect((ResizingRectPos)i));
        }
    }

    painter->drawEllipse(boundingRect()-QMarginsF(5,5,5,5));

    Q_UNUSED(option)
    Q_UNUSED(widget)
}

void DiagramItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    switch (resizeFlag) {
    case Top:
        resize(false, false, true, true, event);
        break;
    case Left:
        resize(true,true,false,false,event);
        break;
    case Right:
        resize(true,false,false,false,event);
        break;
    case Bottom:
        resize(false,false,true,false,event);
        break;
    case TopLeft:
        resize(true,true,true,true,event);
        break;
    case TopRight:
        resize(true,false,true,true,event);
        break;
    case BottomLeft:
        resize(true,true,true,false,event);
        break;
    case BottomRight:
        resize(true,false,true,false,event);
        break;
    default: QGraphicsItem::mouseMoveEvent(event);
    }
    emit Changed();
//        emit ItemPos(top_left_point, event->scenePos());
}

void DiagramItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    for(int i=Top; i <= BottomRight; i++){
        if(getResizingRect((ResizingRectPos)i).contains(event->pos())) resizeFlag = i;
    }
    QGraphicsItem::mousePressEvent(event);
}

void DiagramItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    resizeFlag = 0;
    QGraphicsItem::mouseReleaseEvent(event);
}

//Returns a 10x10 square with the appropriate DiagramItem coordinate
QRectF DiagramItem::getResizingRect(ResizingRectPos RectPos) const
{
    //DiagramItem rect
//    QRectF tmpRect = boundingRect();
    //ResingRect size
//
    switch(RectPos) {
        case Top:
//            return QRectF(tmpRect.center().x()-5, tmpRect.top(), RESIZING_RECT_WIDTH,
//                          RESIZING_RECT_WIDTH);
            return QRectF(size.width()/2, 0, RESIZING_RECT_WIDTH, RESIZING_RECT_WIDTH);
        case Left:
//            return QRectF(tmpRect.left(), tmpRect.center().y()-5, RESIZING_RECT_WIDTH,
//                          RESIZING_RECT_WIDTH);
            return QRectF(0, size.height()/2, RESIZING_RECT_WIDTH, RESIZING_RECT_WIDTH);
        case Right:
            return QRectF(size.width(), size.height()/2, RESIZING_RECT_WIDTH,
                          RESIZING_RECT_WIDTH);
        case Bottom:
            return QRectF(size.width()/2, size.height(), RESIZING_RECT_WIDTH,
                          RESIZING_RECT_WIDTH);
        case TopLeft:
            return QRectF(0, 0, RESIZING_RECT_WIDTH, RESIZING_RECT_WIDTH);
        case TopRight:
            return QRectF(size.width(), 0, RESIZING_RECT_WIDTH, RESIZING_RECT_WIDTH);
        case BottomLeft:
            return QRectF(0, size.height(), RESIZING_RECT_WIDTH, RESIZING_RECT_WIDTH);
        case BottomRight:
            return QRectF(size.width(), size.height(), RESIZING_RECT_WIDTH, RESIZING_RECT_WIDTH);
    }
}

void DiagramItem::resize(bool changeWidth, bool leftSide, bool changeHeight,
                    bool topSide, QGraphicsSceneMouseEvent *event)
{
    prepareGeometryChange();
    //Changing width
    if(changeWidth) {
        //Setting previous width
        double prev_width = size.width();
        //Setting current width
        double cur_width = prev_width + (leftSide ? -1 : 1)
                * (event->pos().x()-event->lastPos().x());

        if(cur_width < MINIMAL_DiagramItem_WIDTH) {
            size.setWidth(MINIMAL_DiagramItem_WIDTH);
            //Changing x if leftSide
            if(leftSide) this->setX(this->x()+prev_width-MINIMAL_DiagramItem_WIDTH);
        } else {
            //When new width is valid
            if(leftSide){
                if(prev_width != MINIMAL_DiagramItem_WIDTH || event->scenePos().x() < this->x()){
                    //If the event is to the left of DiagramItem position
                    size.setWidth(cur_width);
                    this->setX(this->x()-(cur_width-prev_width));
                }
            } else {
                if(prev_width != MINIMAL_DiagramItem_WIDTH ||
                        event->scenePos().x() > this->x()+prev_width){
                    //If the event is to the right of DiagramItem position
                    size.setWidth(cur_width);
                }
            }
        }
    }

    //Changing height
    if(changeHeight)
    {
        //Setting previous height
        double prev_height = size.height();
        //Setting current height
        double cur_height = prev_height + (topSide ? -1 : 1)
                * (event->pos().y()-event->lastPos().y());

        if(cur_height < MINIMAL_DiagramItem_HEIGHT){
            size.setHeight(MINIMAL_DiagramItem_HEIGHT);
            if(topSide) this->setY(this->y()+prev_height-MINIMAL_DiagramItem_HEIGHT);
        } else{
            //When new height is valid
            if(topSide){
                if(prev_height != MINIMAL_DiagramItem_HEIGHT || event->scenePos().y() < this->y()){
                    //If the event is upper than DiagramItem position
                    size.setHeight(cur_height);
                    this->setY(this->y()-(cur_height-prev_height));
                }
            } else{
                if(prev_height != MINIMAL_DiagramItem_HEIGHT ||
                        event->scenePos().y() > this->y()+prev_height){
                    //If the event is lower than DiagramItem position
                    size.setHeight(cur_height);
                }
            }
        }
    }
}

QPixmap DiagramItem::icon() const
{
    QPixmap pixmap(250, 250);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::black, 8));
//    painter.translate(125, 125);
    QPainterPath path;
    path.addRect(10, 50, 230, 150);
    painter.drawPath(path);

    return pixmap;
}