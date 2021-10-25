#include "regularvertex.h"
#include <QDebug>

RegularVertex::RegularVertex(VertexType type, QGraphicsItem* parentItem, QObject *parent)
    : DiagramItem(ItemType::Regular, parentItem, parent), vType(type)
{
    setBrush(QBrush(Qt::white));
    setPen(QPen(QBrush(Qt::black), 4));
}

QPainterPath RegularVertex::shape() const
{
    QPainterPath path = DiagramItem::shape();
    QRectF tmpR(QPointF(0, 0), Size());
    switch (vType) {
    case Process: {
        path.addRect(tmpR);
        break;
    }
    case Data: {
        path.moveTo(tmpR.bottomLeft());
        path.lineTo(tmpR.left()+tmpR.width()*0.125, tmpR.top());
        path.lineTo(tmpR.topRight());
        path.lineTo(tmpR.right()-tmpR.width()*0.125, tmpR.bottom());
        path.lineTo(tmpR.bottomLeft());
        break;
    }
    case Terminator: {
        path.addRoundedRect(tmpR, tmpR.width()*0.125, tmpR.height()*0.5);
        break;
    }
    }
    return path;
}

QPixmap RegularVertex::icon()
{
    QPixmap pic(300, 200);
    pic.fill(Qt::transparent);

    QSizeF old = Size();
    setSize(QSizeF(280, 180));

    QPainter painter(&pic);
    painter.setPen(QPen(Qt::black, 8));
    painter.translate(10, 10);
    painter.drawPath(shape());

    setSize(old);

    return pic;
}

void RegularVertex::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Pen());
    painter->setBrush(Brush());

    painter->drawPath(shape());

    if(this->isSelected()) {
        drawResizingRects(painter);
    }

    Q_UNUSED(option)
    Q_UNUSED(widget)
}
