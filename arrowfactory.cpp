#include "arrowfactory.h"
#include <QDebug>
Arrow *ArrowFactory::createArrow(QLineF line, DiagramItem *startItem, DiagramItem *endItem) const
{
    if((startItem == endItem) && startItem) {
        return nullptr;
    }

    return new Arrow(line, startItem, endItem);
}
