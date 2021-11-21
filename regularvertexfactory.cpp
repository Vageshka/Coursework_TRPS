#include "regularvertexfactory.h"

#include <regularvertex.h>

RegularVertexFactory::RegularVertexFactory()
{

}

DiagramItem *RegularVertexFactory::create(int subtype) const
{
    if(!subtypeIsValid(subtype)) {
        return nullptr;
    }

    return new RegularVertex(static_cast<RegularVertex::VertexType>(subtype));
}

bool RegularVertexFactory::subtypeIsValid(int subtype) const
{
    if((subtype < RegularVertex::begin) || (subtype > RegularVertex::end)) {
        return false;
    }

    return true;
}
