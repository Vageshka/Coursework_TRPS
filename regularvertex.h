#ifndef REGULARVERTEX_H
#define REGULARVERTEX_H

#include <diagramitem.h>

class RegularVertex : public DiagramItem
{
public:
    enum VertexType {
        begin,
        Process = begin,
        Data, Terminator,
        end = Terminator
    };

    RegularVertex(VertexType type, QGraphicsItem* parentItem = nullptr, QObject *parent = nullptr);
    virtual QPainterPath shape() const override;
    virtual QPixmap icon() override;

protected:
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option,
                       QWidget * widget = 0) override;

private:
    VertexType vType;
};

#endif // REGULARVERTEX_H
