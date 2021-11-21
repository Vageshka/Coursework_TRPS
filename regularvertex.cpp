#include "regularvertex.h"
#include <QDebug>
#include <QGraphicsScene>

RegularVertex::RegularVertex(VertexType type, QGraphicsItem* parentItem, QObject *parent)
    : DiagramItem(ItemType::Regular, parentItem, parent)
{
    setSubtype(type);
    setText("");
    setBrush(QBrush(Qt::white));
    setPen(QPen(QBrush(Qt::black), 4));
    RegularVertex::updatePolygon();
}

QPainterPath RegularVertex::shape() const
{
    QPainterPath path = DiagramItem::shape();
    path.addPolygon(itemPolygon());
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
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::HighQualityAntialiasing);

//    painter->setPen(Pen());
    QPen pen = Pen();
    pen.setJoinStyle(Qt::MiterJoin);
    pen.setMiterLimit(5);
    painter->setPen(pen);
    painter->setBrush(Brush());

    painter->drawPolygon(itemPolygon());

    if(this->isSelected()) {
        drawResizingRects(painter);
    }

    painter->setFont(font());
//    auto pen = Pen();
    pen.setColor(textColor());
    painter->setPen(pen);
    painter->drawText(boundingRect(), Qt::AlignCenter, Text());

    Q_UNUSED(option)
    Q_UNUSED(widget)
}

void RegularVertex::updatePolygon()
{
    QPainterPath path;
    QRectF tmpR(QPointF(0, 0), Size());
    switch (static_cast<VertexType>(itemSubtype())) {
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
    case Ellipse: {
        path.addEllipse(tmpR);
        break;
    }
    case Romb: {
        path.moveTo(tmpR.left(), tmpR.center().y());
        path.lineTo(tmpR.center().x(), tmpR.top());
        path.lineTo(tmpR.right(), tmpR.center().y());
        path.lineTo(tmpR.center().x(), tmpR.bottom());
    }
    }

    setPolygon(path.toFillPolygon());
}
