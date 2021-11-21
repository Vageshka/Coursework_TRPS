#ifndef REGULARVERTEXFACTORY_H
#define REGULARVERTEXFACTORY_H

#include <idiagramitemfactory.h>

class RegularVertexFactory : public IDiagramItemFactory
{
public:
    RegularVertexFactory();

    virtual DiagramItem *create(int subtype) const override;
    virtual bool subtypeIsValid(int subtype) const override;
};

#endif // REGULARVERTEXFACTORY_H
