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


void Edge::addAssociation( Association* ass){

    bool found = false;
    for( int i=0; i< associations.size(); i++)
        if( ( associations.at(i)->fromID == ass->fromID ) && ( associations.at(i)->toID == ass->toID ) )
            found = true;
    if( !found )
        associations << ass;
}
