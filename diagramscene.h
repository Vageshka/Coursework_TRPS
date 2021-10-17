#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include <QGraphicsScene>
#include <QObject>
#include "diagramitem.h"
#include "paper.h"

class DiagramScene : public QGraphicsScene
{
    Q_OBJECT
public:
    enum Mode { MoveItem, AddItem, AddArrow };

    Paper *paper;

    explicit DiagramScene(QObject *parent = nullptr);

signals:

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
//    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

private:
    Mode state;
};

#endif // DIAGRAMSCENE_H
