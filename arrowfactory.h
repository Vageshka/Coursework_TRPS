#ifndef ARROWFACTORY_H
#define ARROWFACTORY_H

#include <arrow.h>

class ArrowFactory
{
public:
    virtual Arrow *createArrow(QLineF line, DiagramItem *startItem = nullptr,
                               DiagramItem *endItem = nullptr) const;

    virtual ~ArrowFactory() = default;
};

#endif // ARROWFACTORY_H
