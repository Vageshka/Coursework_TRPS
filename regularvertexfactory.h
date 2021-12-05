#ifndef REGULARVERTEXFACTORY_H
#define REGULARVERTEXFACTORY_H

#include <idiagramitemfactory.h>
#include <regularvertex.h>

class RegularVertexFactory : public IDiagramItemFactory
{
public:
    RegularVertexFactory();

    virtual DiagramItem *createItem(int subtype) const override;
    virtual bool itemSubtypeIsValid(int subtype) const override;
};

#endif // REGULARVERTEXFACTORY_H
