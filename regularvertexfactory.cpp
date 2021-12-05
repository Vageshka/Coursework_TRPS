#include "regularvertexfactory.h"

#include <regularvertex.h>

RegularVertexFactory::RegularVertexFactory()
{

}

DiagramItem *RegularVertexFactory::createItem(int subtype) const
{
    if(!itemSubtypeIsValid(subtype)) {
        return nullptr;
    }

    return new RegularVertex(static_cast<RegularVertex::VertexType>(subtype));
}

bool RegularVertexFactory::itemSubtypeIsValid(int subtype) const
{
    if((subtype < RegularVertex::begin) || (subtype > RegularVertex::end)) {
        return false;
    }

    return true;
}
