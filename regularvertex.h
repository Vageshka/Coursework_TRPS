#ifndef REGULARVERTEX_H
#define REGULARVERTEX_H

#include <diagramitem.h>

class RegularVertex : public DiagramItem
{
public:
    enum VertexType {
        begin,
        Process = begin,
        Romb, Data,
        Terminator, Ellipse,
        end = Ellipse
    };

    RegularVertex(VertexType type, QGraphicsItem* parentItem = nullptr, QObject *parent = nullptr);
    virtual QPainterPath shape() const override;
    virtual QPixmap icon() override;

protected:
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option,
                       QWidget * widget = 0) override;
    virtual void updatePolygon() override;
};

#endif // REGULARVERTEX_H
