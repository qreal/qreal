/** @file entity.cpp
 * 	@brief Описания вспомогательных классов сущностей
 * */
#include "entity.h"

void Entity::addParent( QString id ){
    
    if( !parents.contains(id) )
        parents << id;
}

void Entity::addChild( QString id ){
    if( !children.contains(id) )
        children << id;
}

void Entity::addProperty( QString name, QString type ){

    if( !name.isEmpty())
        if( !properties.contains(QPair<QString, QString>(name, type)))
            properties << QPair<QString, QString>(name, type);
}


void Edge::addAssociation( Association* ass){

    bool found = false;
    for( int i=0; i< associations.size(); i++)
        if( ( associations.at(i)->from == ass->from ) && ( associations.at(i)->to == ass->to ) )
            found = true;
    if( !found )
        associations.push_back(ass);
}
