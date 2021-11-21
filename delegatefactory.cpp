#include "delegatefactory.h"

DelegateFactory::DelegateFactory() : factory_(nullptr) {}

DiagramItem *DelegateFactory::create(int subtype) const
{
    if(!factoryIsSet()) {
        return nullptr;
    }

    return factory_->create(subtype);
}

bool DelegateFactory::subtypeIsValid(int subtype) const
{
    return factory_->subtypeIsValid(subtype);
}

bool DelegateFactory::setFactory(DiagramItem::ItemType type)
{
    if(factoryIsSet()) {
        delete factory_;
        factory_ = nullptr;
    }

    switch (type) {
    case DiagramItem::Regular: {
        factory_ = new RegularVertexFactory;
        break;
    }
    default: {
        return false;
    }
    }

    return true;
}

bool DelegateFactory::factoryIsSet() const
{
    return factory_;
}
