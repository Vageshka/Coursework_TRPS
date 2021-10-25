#include "arrow.h"

#include <QPainter>
#include <QPen>
#include <QtMath>

const int MAX_HEAD_LENGTH = 20;
const int MAX_HEAD_ARC = 60;

Arrow::Arrow(QLineF line, DiagramItem *startItem, DiagramItem *endItem, QGraphicsItem *parent) :
    QGraphicsLineItem(parent), myStartItem(startItem), myEndItem(endItem)
{
    headType = obtuseFilled;
    headLength = 20;
    head = getArrowheadPoly(headType);
    pen.setColor(Qt::black);
    pen.setWidth(2);

    setLine(QLineF( startItem ? startItem->centerPos() : line.p1(),
                    endItem ? endItem->centerPos() : line.p2()));
}

QRectF Arrow::boundingRect() const
{
    qreal delta = pen.width() + MAX_HEAD_LENGTH *
            sin(MAX_HEAD_ARC * M_PI / 180);

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                          line().p2().y() - line().p1().y()))
            .normalized()
            .adjusted(-delta, -delta, delta, delta);
}

QPainterPath Arrow::shape() const
{
    QPainterPath path = QGraphicsLineItem::shape();
    path.addPolygon(head);
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
    QPointF startPoint = startItem() ? startItem()->centerPos() : line().p1();
    QPointF endPoint = endItem() ? endItem()->centerPos() : line().p2();
    QLineF centerLine(startPoint, endPoint);
    painter->setPen(pen);
    painter->drawLine(line());

    QPointF intersectionP = myEndItem ? findIntersection(centerLine, myEndItem) : endPoint;
    QTransform t;
    t.translate(intersectionP.x(), intersectionP.y());
    t.rotate(-centerLine.angle());

    drawArrowhead(painter, headType, head, t);
}

QPointF Arrow::findIntersection(const QLineF &centerLine, DiagramItem *item)
{
    bool selection = item->isSelected();
    item->setSelected(false);

    QPolygonF itemPolygon = item->shape().toFillPolygon();
    QPointF p1 = itemPolygon.first() + item->pos();
    QPointF intersectPoint;
    for (int i = 1; i < itemPolygon.count(); ++i) {
        QPointF p2 = itemPolygon.at(i) + item->pos();
        QLineF polyLine = QLineF(p1, p2);
        QLineF::IntersectionType intersectionType =
            polyLine.intersects(centerLine, &intersectPoint);
        if (intersectionType == QLineF::BoundedIntersection)
            break;
        p1 = p2;
    }

    item->setSelected(selection);
    return intersectPoint;
}

QPolygonF Arrow::getArrowheadPoly(ArrowheadType type)
{
    QPolygonF poly;
    QPointF tip(0, 0);

    switch (type) {
    case noHead: {
        break;
    }
    case acute:
    case acuteEmpty:
    case acuteFilled: {
        double dx = headLength;
        double dy = headLength * tan(M_PI/12);
        poly << QPointF(-dx, -dy) << tip << QPointF(-dx, dy);
        break;
    }
    case obtuse:
    case obtuseEmpty:
    case obtuseFilled: {
        double dx = headLength;
        double dy = headLength * tan(M_PI/6);
        poly << QPointF(-dx, -dy) << tip << QPointF(-dx, dy);
        break;
    }
    case rhombEmpty:
    case rhombFilled: {
        double dx = headLength * 0.5;
        double dy = headLength * tan(M_PI/12);
        poly << QPointF(-2*dx, 0) << QPointF(-dx, -dy) << tip
                << QPointF(-dx, dy);
        break;
    }
    }
    return poly;
}

void Arrow::drawArrowhead(QPainter *painter, ArrowheadType type, const QPolygonF &poly,
                            const QTransform &t)
{
    QPolygonF transformed = t.map(poly);
    switch (type) {
    case noHead: {
        break;
    }
    case acute:
    case obtuse: {
        painter->drawPolyline(transformed);
        break;
    }
    case acuteEmpty:
    case obtuseEmpty:
    case rhombEmpty: {
        painter->setBrush(QBrush(Qt::white));
        painter->drawPolygon(transformed);
        break;
    }
    case acuteFilled:
    case obtuseFilled:
    case rhombFilled: {
        painter->setBrush(QBrush(Qt::black));
        painter->drawPolygon(transformed);
        break;
    }
    }
}
