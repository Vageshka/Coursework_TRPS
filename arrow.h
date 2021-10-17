#ifndef ARROW_H
#define ARROW_H

#include <QObject>
#include <QGraphicsLineItem>
#include <diagramitem.h>

class Arrow : public QObject, public QGraphicsLineItem
{
    Q_OBJECT
public:
    Arrow(QLineF line, DiagramItem *startItem = nullptr, DiagramItem *endItem = nullptr,
          QGraphicsItem *parent = nullptr);

//    int type() const override { return Type; }
    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    DiagramItem *startItem() const { return myStartItem; }
    void setStartItem(DiagramItem *item) { myStartItem = item; }
    DiagramItem *endItem() const { return myEndItem; }
    void setEndItem(DiagramItem *item) { myEndItem = item; }

    void updatePosition();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override;

private:
    DiagramItem *myStartItem;
    DiagramItem *myEndItem;
    QPolygonF arrowHead;
//    QColor myColor = Qt::black;

};

#endif // ARROW_H
