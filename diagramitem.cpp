#include "diagramitem.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QDebug>

const auto DIAGRAMITEM_SIZE = QSize(150, 100);
const auto RESIZING_RECT_WIDTH = 10;
const auto MINIMAL_DIAGRAMITEM_WIDTH = 50;
const auto MINIMAL_DIAGRAMITEM_HEIGHT = 50;

DiagramItem::DiagramItem(ItemType t, QGraphicsItem* parentItem, QObject *parent)
    : QObject(parent), QGraphicsItem(parentItem), type_(t)
{
    size_ = DIAGRAMITEM_SIZE;
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setZValue(1000);
}

void DiagramItem::setSize(const QSizeF &size) {
    prepareGeometryChange();
    size_ = size;
    if(size_.width() < MINIMAL_DIAGRAMITEM_WIDTH) {
        size_.setWidth(MINIMAL_DIAGRAMITEM_WIDTH);
    }
    if(size_.height() < MINIMAL_DIAGRAMITEM_HEIGHT) {
        size_.setHeight(MINIMAL_DIAGRAMITEM_HEIGHT);
    }
    updatePolygon();
}

void DiagramItem::setWidth(double w)
{
    prepareGeometryChange();
    if(w < MINIMAL_DIAGRAMITEM_WIDTH) {
        size_.setWidth(MINIMAL_DIAGRAMITEM_WIDTH);
    } else {
        size_.setWidth(w);
    }
    updatePolygon();
}

void DiagramItem::setHeight(double h)
{
    prepareGeometryChange();
    if(h < MINIMAL_DIAGRAMITEM_HEIGHT) {
        size_.setHeight(MINIMAL_DIAGRAMITEM_HEIGHT);
    } else {
        size_.setHeight(h);
    }
    updatePolygon();
}

QRectF DiagramItem::boundingRect() const
{
    double addition = RESIZING_RECT_WIDTH * 0.5 + Pen().widthF()*5 + 2;

    return QRectF (QPointF(0,0), size_).marginsAdded(QMargins(
                                                        addition, addition,
                                                        addition, addition));
}

QPainterPath DiagramItem::shape() const
{
    QPainterPath path;
    if(isSelected()){
        for(int i=Top; i <= BottomRight; i++){
            path.addRect((getResizingRect((ResizingRectPos)i)));
        }
    }

    return path;
}

void DiagramItem::drawResizingRects(QPainter *painter)
{
    painter->setBrush(QBrush(QColor(135, 206, 250)));
    painter->setPen(QPen(QBrush(Qt::black), 2));

    for(int i=Top; i <= BottomRight; i++){
        painter->drawRect(getResizingRect((ResizingRectPos)i));
    }
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
    int half = RESIZING_RECT_WIDTH * 0.5;
    switch(RectPos) {
        case Top:
            return QRectF(size_.width()*0.5-half, -half,
                          RESIZING_RECT_WIDTH, RESIZING_RECT_WIDTH);
        case Left:
            return QRectF(-half, size_.height()*0.5-half,
                          RESIZING_RECT_WIDTH, RESIZING_RECT_WIDTH);
        case Right:
            return QRectF(size_.width()-half, size_.height()*0.5-half,
                          RESIZING_RECT_WIDTH, RESIZING_RECT_WIDTH);
        case Bottom:
            return QRectF(size_.width()*0.5-half, size_.height()-half,
                          RESIZING_RECT_WIDTH, RESIZING_RECT_WIDTH);
        case TopLeft:
            return QRectF(-half, -half, RESIZING_RECT_WIDTH, RESIZING_RECT_WIDTH);
        case TopRight:
            return QRectF(size_.width()-half, -half, RESIZING_RECT_WIDTH, RESIZING_RECT_WIDTH);
        case BottomLeft:
            return QRectF(-half, size_.height()-half, RESIZING_RECT_WIDTH, RESIZING_RECT_WIDTH);
        case BottomRight:
            return QRectF(size_.width()-half, size_.height()-half,
                          RESIZING_RECT_WIDTH, RESIZING_RECT_WIDTH);
    }
}

void DiagramItem::resize(bool changeWidth, bool leftSide, bool changeHeight,
                    bool topSide, QGraphicsSceneMouseEvent *event)
{
    prepareGeometryChange();
    //Changing width
    if(changeWidth) {
        //Setting previous width
        double prev_width = size_.width();
        //Setting current width
        double cur_width = prev_width + (leftSide ? -1 : 1)
                * (event->pos().x()-event->lastPos().x());

        if(cur_width < MINIMAL_DIAGRAMITEM_WIDTH) {
            size_.setWidth(MINIMAL_DIAGRAMITEM_WIDTH);
            //Changing x if leftSide
            if(leftSide) this->setX(this->x()+prev_width-MINIMAL_DIAGRAMITEM_WIDTH);
        } else {
            //When new width is valid
            if(leftSide){
                if(prev_width != MINIMAL_DIAGRAMITEM_WIDTH || event->scenePos().x() < this->x()){
                    //If the event is to the left of DiagramItem position
                    size_.setWidth(cur_width);
                    this->setX(this->x()-(cur_width-prev_width));
                }
            } else {
                if(prev_width != MINIMAL_DIAGRAMITEM_WIDTH ||
                        event->scenePos().x() > this->x()+prev_width){
                    //If the event is to the right of DiagramItem position
                    size_.setWidth(cur_width);
                }
            }
        }
    }

    //Changing height
    if(changeHeight)
    {
        //Setting previous height
        double prev_height = size_.height();
        //Setting current height
        double cur_height = prev_height + (topSide ? -1 : 1)
                * (event->pos().y()-event->lastPos().y());

        if(cur_height < MINIMAL_DIAGRAMITEM_HEIGHT){
            size_.setHeight(MINIMAL_DIAGRAMITEM_HEIGHT);
            if(topSide) this->setY(this->y()+prev_height-MINIMAL_DIAGRAMITEM_HEIGHT);
        } else{
            //When new height is valid
            if(topSide){
                if(prev_height != MINIMAL_DIAGRAMITEM_HEIGHT || event->scenePos().y() < this->y()){
                    //If the event is upper than DiagramItem position
                    size_.setHeight(cur_height);
                    this->setY(this->y()-(cur_height-prev_height));
                }
            } else{
                if(prev_height != MINIMAL_DIAGRAMITEM_HEIGHT ||
                        event->scenePos().y() > this->y()+prev_height){
                    //If the event is lower than DiagramItem position
                    size_.setHeight(cur_height);
                }
            }
        }
    }
    updatePolygon();
}
