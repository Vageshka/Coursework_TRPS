#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

#include <QBrush>
#include <QGraphicsItem>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QVector>

class DiagramItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    enum ResizingRectPos {
        Top = 1, Bottom,
        Left, Right,
        TopLeft, TopRight,
        BottomLeft, BottomRight
    };
    enum ItemType {
        begin,
        Regular = begin,
        Container,
        end = Container
    };

//    enum DiagramType { X };

    DiagramItem(ItemType type, QGraphicsItem* parentItem = nullptr, QObject *parent = nullptr);
    void setSize(const QSizeF &size) { this->size = size; };
    void setText(const QString &text) { this->text = text; };
    void setBrush(const QBrush &brush) { this->brush = brush; };
    void setPen(const QPen &pen) { this->pen = QPen(pen); };
//    void addArrow(Arrow *arr);
//    void removeArrow(Arrow *arr);

    const QSizeF& Size() const { return size; };
    const QString& Text() const { return text; };
    const QBrush& Brush() const { return brush; };
    const QPen& Pen() const { return pen; };
    QPointF centerPos() const { return mapToScene(boundingRect().center()); }

    virtual QPixmap icon() = 0;

    unsigned int resizeFlag = 0;

    QRectF boundingRect() const override;
    virtual QPainterPath shape() const override;

protected:
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option,
                       QWidget * widget = 0) = 0;

    void drawResizingRects(QPainter *painter);

    QRectF getResizingRect(ResizingRectPos RectPos) const;
    void resize(bool changeWidth, bool leftSide, bool changeHeight,
                bool topSide, QGraphicsSceneMouseEvent *event);

    void mouseMoveEvent(QGraphicsSceneMouseEvent * event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void updateArrows();

private:
//    QVector<Arrow *> arrows;
    ItemType type;
    QSizeF size;
    QString text;
    QBrush brush;
    QPen pen;
};

#endif // DIAGRAMITEM_H
