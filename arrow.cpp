#include "arrow.h"

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QPainter>
#include <QPen>
#include <QtMath>
#include<QDebug>

const int MAX_HEAD_LENGTH = 100;
const int MAX_HEAD_ARC = 30;
const int MOVING_CIRCLES_RADIUS = 5;

Arrow::Arrow(QLineF line, DiagramItem *startItem, DiagramItem *endItem, QGraphicsItem *parent) :
    QGraphicsLineItem(parent), myStartItem(startItem), myEndItem(endItem)
{
    headType = noHead;
    mType = none;
    headLength = 20;
    head = getArrowheadPoly(headType);
    pen.setColor(Qt::black);
    pen.setWidth(2);

    setFlag(ItemIsSelectable);

    setLine(QLineF( startItem ? startItem->centerPos() : line.p1(),
                    endItem ? endItem->centerPos() : line.p2()));
}

QRectF Arrow::boundingRect() const
{
    qreal delta = pen.width() + headLength *
            sin(MAX_HEAD_ARC * M_PI / 180);

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                          line().p2().y() - line().p1().y()))
            .normalized()
            .adjusted(-delta, -delta, delta, delta);
}

QPainterPath Arrow::shape() const
{
    QPainterPath path = QGraphicsLineItem::shape();

    if(isSelected()) {
        path.addEllipse(line().p1(), MOVING_CIRCLES_RADIUS, MOVING_CIRCLES_RADIUS);
        path.addEllipse(line().p2(), MOVING_CIRCLES_RADIUS, MOVING_CIRCLES_RADIUS);
    }
    path.addPolygon(head);
    return path;
}

void Arrow::updateItems()
{
    QTransform t(0,0,0,0,0,0);
    this->setVisible(false);
    myStartItem = dynamic_cast<DiagramItem*>(scene()->itemAt(line().p1(), t));
    myEndItem = dynamic_cast<DiagramItem*>(scene()->itemAt(line().p2(), t));
    this->setVisible(true);
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

QPixmap Arrow::arrowheadIcon(ArrowheadType t)
{
    QPixmap pic(100, 100);
    pic.fill(Qt::transparent);

    QPainter painter(&pic);
    painter.setPen(QPen(Qt::black, 8));
    QTransform tranform;
    painter.drawLine(0, 50, 100, 50);
    tranform.translate(100, 50);
    setArrowheadLength(60);
    drawArrowhead(&painter, t, getArrowheadPoly(t), tranform);
    painter.translate(10, 10);
    painter.drawPath(shape());

    return pic;
}

void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::HighQualityAntialiasing);

    QPointF startPoint = startItem() ? startItem()->centerPos() : line().p1();
    QPointF endPoint = endItem() ? endItem()->centerPos() : line().p2();
    QLineF centerLine(startPoint, endPoint);
    setLine(centerLine);
    painter->setPen(pen);
    painter->drawLine(line());

    if(!(myStartItem && myEndItem && myStartItem->collidesWithItem(myEndItem))) {
        QPointF intersectionP = myEndItem
                ? findIntersection(centerLine, myEndItem, myEndItem->Pen().widthF()/2)
                : endPoint;
        QTransform t;
        t.translate(intersectionP.x(), intersectionP.y());
        t.rotate(-centerLine.angle());
        drawArrowhead(painter, headType, head, t);
    }

    if(isSelected()) {
        painter->setBrush(QBrush(QColor(135, 206, 250)));
        painter->setPen(QPen(QBrush(Qt::black), 2));

        painter->drawEllipse(line().p1(), MOVING_CIRCLES_RADIUS, MOVING_CIRCLES_RADIUS);
        painter->drawEllipse(line().p2(), MOVING_CIRCLES_RADIUS, MOVING_CIRCLES_RADIUS);
    }
}

QPointF Arrow::findIntersection(const QLineF &centerLine, DiagramItem *item, double half_border)
{
    QPolygonF itemPolygon = item->itemPolygon();
    QPointF p1 = itemPolygon.first() + item->pos();
    QPointF intersectPoint;
    for (int i = 1; i < itemPolygon.count(); ++i) {
        QPointF p2 = itemPolygon.at(i) + item->pos();
        QLineF polyLine = QLineF(p1, p2);
        QLineF::IntersectionType intersectionType =
            polyLine.intersects(centerLine, &intersectPoint);
        if (intersectionType == QLineF::BoundedIntersection){
            double inside_boreder = half_border
                    / sin(qDegreesToRadians(centerLine.angleTo(polyLine)));
            intersectPoint.rx() += inside_boreder * cos(qDegreesToRadians(centerLine.angle()-180));
            intersectPoint.ry() -= inside_boreder * sin(qDegreesToRadians(centerLine.angle()-180));
            break;
        }
        p1 = p2;
    }

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

void Arrow::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    mType = none;

    if(sqrt(pow(event->pos().x()-line().p1().x(), 2) +
                pow(event->pos().y()-line().p1().y(), 2)) <= MOVING_CIRCLES_RADIUS) {
        mType = moveP1;
    }

    if(sqrt(pow(event->pos().x()-line().p2().x(), 2) +
                pow(event->pos().y()-line().p2().y(), 2)) <= MOVING_CIRCLES_RADIUS) {
        mType = moveP2;
    }

    setSelected(true);
    QGraphicsLineItem::mousePressEvent(event);
}

void Arrow::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(mType == none) {
        return;
    }

    auto l = line();

    if(mType == moveP1) {
        myStartItem = nullptr;
        l.setP1(event->pos());
    }
    if(mType == moveP2) {
        myEndItem = nullptr;
        l.setP2(event->pos());
    }

    setLine(l);
}

void Arrow::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(mType == none) {
        return;
    }

    QTransform t(0,0,0,0,0,0);
    this->setVisible(false);
    (mType == moveP1 ? myStartItem : myEndItem)
            = dynamic_cast<DiagramItem*>(scene()->itemAt(event->scenePos(), t));
    this->setVisible(true);

    if(myStartItem == myEndItem) {
        myStartItem = nullptr;
    }
}

QVariant Arrow::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedChange) {
        if (value == true) {
            setZValue(1001);
        }
        else {
            setZValue(0);
        }
    }

    return QGraphicsLineItem::itemChange(change, value);
}
