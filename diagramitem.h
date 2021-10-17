#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

#include <QGraphicsItem>
#include <QPainterPath>

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

//    enum DiagramType { X };

    DiagramItem(QGraphicsItem* parentItem = nullptr, QObject *parent = nullptr);
    void setSize(QSize size) { this->size = size; };
    QSizeF Size() const { return size; };
    void setText(QString text) { this->text = text; };
    QString Text() const { return text; };

    virtual QPixmap icon() const = 0;

    unsigned int resizeFlag = 0;

    QRectF boundingRect() const override;
//    virtual QPainterPath shape() const override;

public slots:

signals:
    void Changed();

protected:
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option,
                       QWidget * widget = 0) override;

    QRectF getResizingRect(ResizingRectPos RectPos) const;
    void resize(bool changeWidth, bool leftSide, bool changeHeight,
                bool topSide, QGraphicsSceneMouseEvent *event);

    void mouseMoveEvent(QGraphicsSceneMouseEvent * event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QSizeF size;
    QString text;
};

#endif // DIAGRAMITEM_H
