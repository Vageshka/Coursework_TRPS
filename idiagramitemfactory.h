#ifndef IDIAGRAMITEMFACTORY_H
#define IDIAGRAMITEMFACTORY_H

#include <diagramitem.h>

class IDiagramItemFactory
{
public:
    virtual DiagramItem* create(int subtype) const = 0;
    virtual bool subtypeIsValid(int subtype) const = 0;

    virtual ~IDiagramItemFactory() = default;
};

#endif // IDIAGRAMITEMFACTORY_H
