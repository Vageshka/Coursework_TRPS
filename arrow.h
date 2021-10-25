#ifndef ARROW_H
#define ARROW_H

#include <QObject>
#include <QGraphicsLineItem>
#include "diagramitem.h"

class Arrow : public QObject, public QGraphicsLineItem
{
    Q_OBJECT
public:
    enum ArrowheadType {
        noHead,
        acute, obtuse,
        acuteEmpty, obtuseEmpty,
        acuteFilled, obtuseFilled,
        rhombEmpty, rhombFilled
    };

    Arrow(QLineF line, DiagramItem *startItem = nullptr, DiagramItem *endItem = nullptr,
            QGraphicsItem *parent = nullptr);

//    int type() const override { return Type; }
    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    void setStartItem(DiagramItem *item) { myStartItem = item; }
    void setEndItem(DiagramItem *item) { myEndItem = item; }
    void setPen(const QPen &p) { pen = p; }
    void clearStartItem() { myStartItem = nullptr; }
    void clearEndItem() { myEndItem = nullptr; }
    void setArrowhead(ArrowheadType type) { headType = type; head = getArrowheadPoly(type); }
    void setArrowheadLength(int length) { headLength = length; }

    const DiagramItem *startItem() const { return myStartItem; }
    const DiagramItem *endItem() const { return myEndItem; }
    const QPen& Pen() const { return pen; };
    ArrowheadType arrowhead() const { return headType; }
    QPolygonF arrowheadPoly() const { return head; }
    int arrowheadLength() const { return headLength; }

    void updatePosition();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override;
    QPointF findIntersection(const QLineF& centerLine, DiagramItem *item);
    QPolygonF getArrowheadPoly(ArrowheadType type);
    void drawArrowhead(QPainter *painter, ArrowheadType type, const QPolygonF &poly,
                        const QTransform &t);

private:
    DiagramItem *myStartItem;
    DiagramItem *myEndItem;
    QPen pen;
    ArrowheadType headType;
    QPolygonF head;
    int headLength;
};

#endif // ARROW_H
