#include "delegatefactory.h"

DelegateFactory::DelegateFactory() : di_factory(nullptr), ar_factory(nullptr) {}

DiagramItem *DelegateFactory::createItem(int subtype) const
{
    if(!itemFactoryIsSet()) {
        return nullptr;
    }

    return di_factory->createItem(subtype);
}

bool DelegateFactory::itemSubtypeIsValid(int subtype) const
{
    return di_factory->itemSubtypeIsValid(subtype);
}

Arrow *DelegateFactory::createArrow(QLineF line, DiagramItem *startItem, DiagramItem *endItem) const
{
    if(!arrowFactoryIsSet()) {
        return nullptr;
    }

    return ar_factory->createArrow(line, startItem, endItem);
}

bool DelegateFactory::setItemFactory(DiagramItem::ItemType type)
{
    if(itemFactoryIsSet()) {
        delete di_factory;
        di_factory = nullptr;
    }

    switch (type) {
    case DiagramItem::Regular: {
        di_factory = new RegularVertexFactory;
        break;
    }
    default: {
        return false;
    }
    }

    return true;
}

bool DelegateFactory::setArrowFactory(Arrow::ArrowType type)
{
    if(arrowFactoryIsSet()) {
        delete ar_factory;
        ar_factory = nullptr;
    }

    switch (type) {
    case Arrow::StraightArrow: {
        ar_factory = new ArrowFactory;
        break;
    }
    default: {
        return false;
    }
    }

    return true;
}

bool DelegateFactory::itemFactoryIsSet() const
{
    return di_factory;
}

bool DelegateFactory::arrowFactoryIsSet() const
{
    return ar_factory;
}
