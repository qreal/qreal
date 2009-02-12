/** @file entity.cpp
 * 	@brief Описания вспомогательных классов сущностей
 * */
#include "entity.h"
#include "generator.h"

bool Entity::addParent( QString parent_id ){
	if (id == parent_id)
	{
		qDebug() << "Element " << id << " is parent to himself?!";
		return false;
	}
	if( !parents.contains(parent_id) )
		parents << parent_id;
	return true;
}

bool Entity::addAllParents( QString parent_id ){
	if (id == parent_id)
	{
		qDebug() << "Element " << id << " is parent to himself?!";
		return false;
	}
	if( !all_parents.contains(parent_id) )
		all_parents << parent_id;
	return true;
}

bool Entity::addAllParents( QStringList &list ){
	for (int i = 0; i < (int) list.size(); i++)
		if (!addAllParents(list.at(i)))
			return false;
	return true;
}


void Entity::addProperty( QString name, QString type ){
	if( !name.isEmpty())
		if( !properties.contains(QPair<QString, QString>(name, type)))
			properties << QPair<QString, QString>(name, type);
}

void Entity::addAllProperties( QString name, QString type ){
	if( !name.isEmpty())
		if( !all_properties.contains(QPair<QString, QString>(name, type)))
			all_properties << QPair<QString, QString>(name, type);
}

void Entity::addAllProperties( QList< QPair<QString, QString> > &list){
	for (int i = 0; i < (int) list.size(); i++)
		addAllProperties(list.at(i).first, list.at(i).second);
}

void Edge::addAssociation( Association* ass){
	bool found = false;

	for( int i=0; i< associations.size(); i++)
		if( ( associations.at(i)->from == ass->from ) && ( associations.at(i)->to == ass->to ) )
			found = true;
	if( !found )
		associations.push_back(ass);
}

bool Entity::propagateAll(Generator *g)
{
	if (propagated) return true;
	if (!all_parents.isEmpty())
	{
		qDebug() << "Cannot process " << id << ": vicious circle!";
		return false;
	}
	for (int i = 0; i < parents.size(); i++)
	{
		Entity *par = g->find(parents.at(i));
		if( !par ){
			qDebug() << "Entity " << parents.at(i) << " in a generalizations list of " << id << "not found";
			return false;
		}

		// Now add parent to all_parents list to detect cycles.
		if (!addAllParents(par->id)) return false;

		// Process parent
		if (!par->propagateAll(g)) return false;
		if (!addAllParents(par->all_parents)) return false;
		addAllProperties(par->all_properties);
	}

	// Add entity's own properties
	addAllProperties(properties);
	propagated = true;
	return true;
}
