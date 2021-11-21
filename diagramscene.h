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
    void setItemType(DiagramItem::ItemType type) { itemType_ = type; }
    void setItemSubtype(int subtype) { itemSubtype_ = subtype; }
    void setArrowheadType(Arrow::ArrowheadType type) { headType = type; };
    void updateArrowsItems();

    Mode curState() const { return state; };
    QList<DiagramItem *> selectedDiagramItems() const;
    QList<Arrow *> selectedArrows() const;
    QList<DiagramItem *> diagramItems() const;
    QList<Arrow *> arrows() const;
    bool isClear();

signals:

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    Mode state;
    QGraphicsLineItem *dashLine;
    QPointF origPoint;
    QGraphicsRectItem *selectingRect;
    DiagramItem *firstItem;
    Arrow::ArrowheadType headType;
    QColor textColor_;
    DiagramItem::ItemType itemType_;
    int itemSubtype_;
};

#endif // DIAGRAMSCENE_H
