#include "typedEntity.h"

TypedEntity::TypedEntity()
{}

void TypedEntity::setType(const QString &type)
{
    mType = type;
}

QString TypedEntity::getType() const
{
    return mType;
}

TypedEntity::~TypedEntity() {}
