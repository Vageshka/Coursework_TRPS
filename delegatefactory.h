#ifndef DELEGATEFACTORY_H
#define DELEGATEFACTORY_H

#include "arrowfactory.h"
#include "regularvertexfactory.h"

class DelegateFactory : public IDiagramItemFactory, public ArrowFactory
{
public:
    DelegateFactory();

    virtual DiagramItem *createItem(int subtype) const override;
    virtual bool itemSubtypeIsValid(int subtype) const override;

    virtual Arrow *createArrow(QLineF line, DiagramItem *startItem = nullptr,
                               DiagramItem *endItem = nullptr) const override;

    bool setItemFactory(DiagramItem::ItemType type);
    bool setArrowFactory(Arrow::ArrowType type);

    bool itemFactoryIsSet() const;
    bool arrowFactoryIsSet() const;

private:
    IDiagramItemFactory *di_factory;
    ArrowFactory *ar_factory;
};

#endif // DELEGATEFACTORY_H
