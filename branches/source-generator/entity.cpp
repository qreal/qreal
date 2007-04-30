#include "entity.h"

void Entity::addParent( QString id ){
    
    if( !parents.contains(id) )
        parents << id;
}


void Entity::addProperty( QString name, QString type ){

    if( !name.isEmpty())
        if( !properties.contains(QPair<QString, QString>(name, type)))
            properties << QPair<QString, QString>(name, type);
}
