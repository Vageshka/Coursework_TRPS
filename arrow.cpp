#include "arrow.h"

#include <QPainter>
#include <QPen>
#include <QtMath>

#define MAX_HEAD_LENGTH 20
#define MAX_HEAD_ARC 60

Arrow::Arrow(QLineF line, DiagramItem *startItem, DiagramItem *endItem, QGraphicsItem *parent) :
    QGraphicsLineItem(parent), myStartItem(startItem), myEndItem(endItem)
{
    setLine(line);
    arrowHead.clear();
}

QRectF Arrow::boundingRect() const
{
    qreal delta = pen().width() + MAX_HEAD_LENGTH *
            sin(MAX_HEAD_ARC * M_PI / 180);

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                          line().p2().y() - line().p1().y()))
            .normalized()
            .adjusted(-delta, -delta, delta, delta);
}

QPainterPath Arrow::shape() const
{
    QPainterPath path = QGraphicsLineItem::shape();
    path.addPolygon(arrowHead);
    return path;
}

void Arrow::updatePosition()
{
    QLineF curLine(line());

    if(myStartItem){
        curLine.setP1(mapFromItem(myStartItem, myStartItem->boundingRect().center()));
    }
    if(myEndItem) {
        curLine.setP2(mapFromItem(myEndItem, myEndItem->boundingRect().center()));
    }
    setLine(curLine);
}

void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawLine(line());
}
