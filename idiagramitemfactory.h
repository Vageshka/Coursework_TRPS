#ifndef IDIAGRAMITEMFACTORY_H
#define IDIAGRAMITEMFACTORY_H

#include <diagramitem.h>

class IDiagramItemFactory
{
public:
    virtual DiagramItem* createItem(int subtype) const = 0;
    virtual bool itemSubtypeIsValid(int subtype) const = 0;

    virtual ~IDiagramItemFactory() = default;
};

#endif // IDIAGRAMITEMFACTORY_H
