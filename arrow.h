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
        noHead, begin = noHead,
        acute, obtuse,
        acuteEmpty, obtuseEmpty,
        acuteFilled, obtuseFilled,
        rhombEmpty, rhombFilled,
        end
    };

    enum MoveType {
        none, moveP1, moveP2
    };

    Arrow(QLineF line, DiagramItem *startItem = nullptr, DiagramItem *endItem = nullptr,
            QGraphicsItem *parent = nullptr);

//    int type() const override { return Type; }
    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    void setStartItem(DiagramItem *item) { myStartItem = item; }
    void setEndItem(DiagramItem *item) { myEndItem = item; }
    void setPen(const QPen &p) { prepareGeometryChange(); pen = p; }
    void clearStartItem() { prepareGeometryChange(); myStartItem = nullptr; }
    void clearEndItem() { prepareGeometryChange(); myEndItem = nullptr; }
    void setArrowhead(ArrowheadType type) { prepareGeometryChange(); headType = type; head = getArrowheadPoly(type); }
    void setArrowheadLength(int length) { prepareGeometryChange(); headLength = length; head = getArrowheadPoly(headType); }
    void updateItems();

    DiagramItem * const startItem() const { return myStartItem; }
    DiagramItem * const endItem() const { return myEndItem; }
    const QPen& Pen() const { return pen; };
    ArrowheadType arrowhead() const { return headType; }
    QPolygonF arrowheadPoly() const { return head; }
    int arrowheadLength() const { return headLength; }

    void updatePosition();
    QPixmap arrowheadIcon(ArrowheadType t);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override;
    QPointF findIntersection(const QLineF& centerLine, DiagramItem *item, double half_border);
    QPolygonF getArrowheadPoly(ArrowheadType type);
    void drawArrowhead(QPainter *painter, ArrowheadType type, const QPolygonF &poly,
                        const QTransform &t);

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    DiagramItem *myStartItem;
    DiagramItem *myEndItem;
    QPen pen;
    ArrowheadType headType;
    QPolygonF head;
    int headLength;
    MoveType mType;
};

#endif // ARROW_H
