#ifndef DELEGATEFACTORY_H
#define DELEGATEFACTORY_H

#include <regularvertexfactory.h>

class DelegateFactory : public IDiagramItemFactory
{
public:
    DelegateFactory();

    virtual DiagramItem *create(int subtype) const override;
    virtual bool subtypeIsValid(int subtype) const override;

    bool setFactory(DiagramItem::ItemType type);
    bool factoryIsSet() const;

private:
    IDiagramItemFactory *factory_;
};

#endif // DELEGATEFACTORY_H
