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
        Regular, begin = Regular,
        Container,
        end = Container
    };

    DiagramItem(ItemType type, QGraphicsItem* parentItem = nullptr, QObject *parent = nullptr);
    virtual ~DiagramItem() = default;

    void setSize(const QSizeF &size);
    void setWidth(double w);
    void setHeight(double h);
    void setText(const QString &text) { prepareGeometryChange(); text_ = text; };
    void setBrush(const QBrush &brush) { prepareGeometryChange(); this->brush = brush; };
    void setPen(const QPen &pen) { prepareGeometryChange(); this->pen = QPen(pen); };
    void setFont(const QFont &f) { prepareGeometryChange(); font_ = f; }
    void setTextColor(const QColor& color) { textColor_ = color; };

    const QSizeF& Size() const { return size_; };
    const QString& Text() const { return text_; };
    const QBrush& Brush() const { return brush; };
    const QPen& Pen() const { return pen; };
    QPointF centerPos() const { return mapToScene(boundingRect().center()); }
    const QFont& font() const { return font_; }
    const QColor& textColor() const {return textColor_;}
    const QPolygonF& itemPolygon() const { return poly_; }
    ItemType itemType() const { return type_; }
    virtual int itemSubtype() const { return subtype_; }


    virtual QPixmap icon() = 0;

    unsigned int resizeFlag = 0;

    QRectF boundingRect() const override;
    virtual QPainterPath shape() const override;

protected:
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option,
                       QWidget * widget = 0) override = 0;

    void drawResizingRects(QPainter *painter);

    QRectF getResizingRect(ResizingRectPos RectPos) const;
    void resize(bool changeWidth, bool leftSide, bool changeHeight,
                bool topSide, QGraphicsSceneMouseEvent *event);

    void mouseMoveEvent(QGraphicsSceneMouseEvent * event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void setPolygon(const QPolygonF& poly) { poly_ = poly; }
    virtual void updatePolygon() = 0;
    void setSubtype(int subtype) { subtype_ = subtype; }

private:
    ItemType type_;
    int subtype_;
    QSizeF size_;
    QString text_;
    QFont font_;
    QBrush brush;
    QPen pen;
    QColor textColor_;
    QPolygonF poly_;
};

#endif // DIAGRAMITEM_H
