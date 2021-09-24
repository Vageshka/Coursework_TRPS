#ifndef VERTEX_H
#define VERTEX_H

#include <QGraphicsItem>

class Vertex : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    Vertex(QGraphicsItem* parentItem = nullptr, QObject *parent = nullptr);
    void setSize(QSize size) { this->size = size; };
    QSizeF getSize() const { return size; };
    void setText(QString text) { this->text = text; };
    QString getText() const { return text; };

    enum ResizingRectPos {
            Top = 1, Bottom,
            Left, Right,
            TopLeft, TopRight,
            BottomLeft, BottomRight
        };

    unsigned int resizeFlag = 0;

public slots:

signals:
    void Changed();

protected:
    QSizeF size;
    QString text;

    QRectF boundingRect() const;
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option,
                       QWidget * widget = 0) = 0;

    QRectF getResizngRect(ResizingRectPos RectPos) const;
    void resize(bool changeWidth, bool leftSide, bool changeHeight,
                bool topSide, QGraphicsSceneMouseEvent *event);

    void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // VERTEX_H
