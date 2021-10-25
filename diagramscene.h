#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include <QGraphicsScene>
#include <QObject>
#include "regularvertex.h"
#include "arrow.h"
#include "paper.h"

class DiagramScene : public QGraphicsScene
{
    Q_OBJECT
public:
    enum Mode { MoveItem, AddItem, AddArrow };

    Paper *paper;

    explicit DiagramScene(QObject *parent = nullptr);
    void setState(Mode state = MoveItem) { this->state = state; };
    void setItemTypeFlags(unsigned int flags) { itemTypeFlags = flags; };

    Mode curState() const { return state; };
    unsigned int curItemTypeFlags() const { return itemTypeFlags; };

signals:

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    DiagramItem *newItem();
    bool itemTypeFlagsValid();

private:
    Mode state;
    unsigned int itemTypeFlags;
    QGraphicsLineItem *dashLine;
    DiagramItem *firstItem;
};

#endif // DIAGRAMSCENE_H
