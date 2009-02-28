/** @file classes.cpp
 * 	@brief Классы, используемые для хранения элементов диаграмм в репозитории
 * */
#include "classes.h"

RepoData::RepoData()
{
	mRoot = new Object("repoRoot", "noType");
	addObject("repoRoot", mRoot);
}

RepoData::~RepoData()
{
	foreach( Object *obj, objects.values() )
		delete obj;
	foreach( Link *link, links.values() )
		delete link;
	objects.clear();
	links.clear();
}

void RepoData::addObject( IdType const &id, Object* obj )
{
	objects[id] = obj;
}

void RepoData::addLink( IdType const &id, Link* link )
{
	links[id] = link;
}

void RepoData::deleteObject( IdType const &id )
{
	objects.remove(id);
}

void RepoData::deleteLink( IdType const &id )
{
	objects.remove(id);
}

int RepoData::getObjectsSize()
{
	foreach( Object* obj, objects)
		qDebug() << "id: " << obj->getId() << ", name: " << obj->getName();
	return objects.size();
}

int RepoData::getLinksSize()
{
	return links.size();
}

Object *RepoData::getObject( IdType const &id )
{
	if( objects.contains(id))
		return objects[id];
	else
		return 0;
}

Link *RepoData::getLink( IdType const &id )
{
	if( links.contains(id) )
		return links[id];
	else
		return 0;
}

QString RepoData::getObjectsByType( TypeIdType const &type )
{
	QString res = "";
	// TODO: гм...
	for( int i=0; i<objects.values().size(); i++ ){
		IdType key = objects.keys()[i];
		Object *obj = objects.value(key);
		if( obj && obj->getType() == type )
			res += QString("%1\t").arg(obj->getId());
	}
	if( !res.isEmpty() )
		res.chop(1);
	return res;
}

QString RepoData::getLinksByType( TypeIdType const &type )
{
	QString res = "";
	for( int i=0; i<links.size(); i++ ){
		IdType key = links.keys()[i];
		Link *link = links.value(key);
		if( link && link->getType() == type )
			res += QString("%1\t").arg(link->getId());
	}

	if( !res.isEmpty() )
		res.chop(1);
	return res;
}

Object* RepoData::getRootObject() const
{
	return mRoot;
}

QList<IdType> RepoData::getAllObjects() const
{
	return objects.keys();
}

QList<IdType> RepoData::getAllLinks() const
{
	return links.keys();
}

// --------------------------------------------------------------------------- //

RepoElement::RepoElement(IdType const &id, TypeIdType const &type)
{
	this->id = id;
	this->type = type;
}

void RepoElement::setName( QString arg )
{
	name = arg;
}

QString RepoElement::getName()
{
	return name;
}

void RepoElement::setDescription( QString arg )
{
	description = arg;
}

QString RepoElement::getDescription()
{
	return description;
}

IdType RepoElement::getId()
{
	return id;
}

void RepoElement::setId(IdType const &id)
{
	this->id = id;
}

TypeIdType RepoElement::getType()
{
	return type;
}

QString RepoElement::parentsToString()
{
	QString res = "";
	foreach (IdType parent, parents)
		res += QString("%1\t").arg(parent);
	res.chop(1);
	return res;
}

void RepoElement::setProperty( QString name, QString val )
{
	props[name] = val;
}

QString RepoElement::getProperty( QString name )
{
	return props.value(name);
}

// --------------------------------------------------------------------------- //

Object::Object(IdType const &id, TypeIdType const &type)
	: RepoElement(id, type)
{
}

int Object::childrenCount()
{
	return nodeChildren.count() + edgeChildren.count();
}

void Object::addNodeChild( IdType const &child )
{
	NodeOnDiagram node(child);
	nodeChildren[child] = node;
}

void Object::addEdgeChild( IdType const &child )
{
	EdgeOnDiagram edge(child);
	edgeChildren[child] = edge;
}

void Object::removeNodeChild( IdType const &child )
{
	foreach( NodeOnDiagram node, nodeChildren)
		if( node.getId() == child ){
			nodeChildren.remove(child);
//			qDebug() << "removed node with id " << child << " from diagram " << this->id;
		}
}

void Object::removeEdgeChild( IdType const &child )
{
	foreach( EdgeOnDiagram edge, edgeChildren)
		if( edge.getId() == child ){
			edgeChildren.remove(child);
//			qDebug() << "removed edge with id " << child << " from diagram " << this->id;
		}
}

QString Object::childrenToString()
{
	QString res = "";
	for( int i=0; i<nodeChildren.values().size(); i++ )
		res += QString("%1\t").arg(nodeChildren.values()[i].getId());
	for( int i=0; i<edgeChildren.values().size(); i++ )
		res += QString("%1\t").arg(edgeChildren.values()[i].getId());
	return res;
}

QString Object::getIncomingLinks()
{
	QString res;
	for( int i=0; i<incomingLinks.size(); i++)
		res += QString("%1\t").arg(incomingLinks.at(i));
	return res;
}

QString Object::getOutcomingLinks()
{
	QString res;
	for( int i=0; i<outcomingLinks.size(); i++)
		res += QString("%1\t").arg(outcomingLinks.at(i));
	return res;
}

QString Object::toString()
{
	QString res = "";
	res += QString("%1\t%2\t%3\t%4\t")
			.arg(type).arg(id).arg(name).arg(description);

	res += QString("%1\t").arg(childrenCount());
	res += childrenToString();

	res += QString("%1\t").arg(incomingLinks.size());
	res += getIncomingLinks();

	res += QString("%1\t").arg(outcomingLinks.size());
	res += getOutcomingLinks();

	res += QString("%1\t").arg(props.size());
	for( int i=0; i<props.keys().size(); i++){
		res += QString("%1;%2\t").arg(props.keys().at(i)).arg(props.value(props.keys().at(i)));
	}
	return res;
}

void Object::addLink( IdType const &id, int dir )
{
	if( dir == INCOMING_LINK ){
		if( !incomingLinks.contains(id) ){
//			qDebug() << "added inc link" << id;
			incomingLinks << id;
		}
	}
	else if ( dir == OUTCOMING_LINK ){
		if( !outcomingLinks.contains(id) ){
//			qDebug() << "added outc link" << id;
			outcomingLinks << id;
		}
	}
}

void Object::removeLink( IdType const &id, int dir )
{
	if( dir == INCOMING_LINK ){
//		qDebug() << "\t\tbefore:" << incomingLinks;
		incomingLinks.removeAll(id);
//		qDebug() << "\t\tafter:" << incomingLinks;
	}
	else if ( dir == OUTCOMING_LINK ){
//		qDebug() << "\t\tbefore:" << outcomingLinks;
		outcomingLinks.removeAll(id);
//		qDebug() << "\t\tafter:" << outcomingLinks;
	}
}

QString Object::getChildPos( IdType const &id )
{
	if( edgeChildren.contains(id) )
		return edgeChildren[id].getPosition();
	else
		return "";
}

bool Object::setChildPos( IdType const &id, QString pos )
{
	if( edgeChildren.contains(id) )
		edgeChildren[id].setPosition(pos);
	else
		return false;
	return true;
}

QPoint Object::getChildCoord( IdType const &id)
{
	if( nodeChildren.contains(id) )
		return nodeChildren[id].getCoord();
	return QPoint();
}

bool Object::setChildCoord( IdType const &id, QPoint p)
{
	if( nodeChildren.contains(id) )
		nodeChildren[id].setCoord(p);
	else
		return false;
	return true;
}

QString Object::getChildConfiguration( IdType const &id)
{
	if( nodeChildren.contains(id) )
		return nodeChildren[id].getConfiguration();
	else if ( edgeChildren.contains(id) )
		return edgeChildren[id].getConfiguration();
	else
		return "";
}

bool Object::setChildConfiguration( IdType const &id, QString conf )
{
	qDebug() << "looking for" << id << "in " << nodeChildren.keys();

	if( nodeChildren.contains(id) )
		nodeChildren[id].setConfiguration(conf);
	else if( edgeChildren.contains(id) )
		edgeChildren[id].setConfiguration(conf);
	else
		return false;
	return true;
}

bool Object::isParentOf(IdType const &id) const
{
	return nodeChildren.contains(id) || edgeChildren.contains(id);
}

// --------------------------------------------------------------------------- //

Link::Link(IdType const &id, TypeIdType const &type)
	: RepoElement(id, type)
{
}

QString Link::getObjects()
{
	QString res = "";
	res += QString("%1\t").arg(objectsTo.size());
	res += getObjectsTo();

	res += QString("%1\t").arg(objectsFrom.size());
	res += getObjectsFrom();
	return res;
}

QString Link::getObjectsFrom()
{
//	qDebug() << "objects from: " << objectsFrom;
	QString res = "";
	for( int i=0; i<objectsFrom.size(); i++)
		res += QString("%1\t").arg(objectsFrom.at(i));
	return res;
}

QString Link::getObjectsTo()
{
	QString res = "";
	for( int i=0; i<objectsTo.size(); i++)
		res += QString("%1\t").arg(objectsTo.at(i));
	return res;
}

QString Link::toString()
{
	QString res = "";
	res += QString("%1\t%2\t%3\t%4\t").arg(type).arg(id).arg(name).arg(description);

	res += getObjects();

	res += QString("%1\t").arg(props.size());
	for( int i=0; i<props.keys().size(); i++){
		res += QString("%1;%2\t").arg(props.keys().at(i)).arg(props.value(props.keys().at(i)));
	}
	return res;
}

void Link::addObjectTo( IdType const &id )
{
	if( !objectsTo.contains( id ) )
		objectsTo << id;
}

void Link::addObjectFrom( IdType const &id )
{
	if( !objectsFrom.contains( id ) )
		objectsFrom << id;
}

void Link::removeObjectTo( IdType const &id )
{
	objectsTo.removeAll(id);
}

void Link::removeObjectFrom( IdType const &id )
{
	objectsFrom.removeAll(id);
}

IdType Link::getFrom()
{
	if( objectsFrom.size() > 0 )
		return objectsFrom.at(0);

	return INVALID_ID;
}

IdType Link::getTo()
{
	if( objectsTo.size() > 0 )
		return objectsTo.at(0);

	return INVALID_ID;
}

// --------------------------------------------------------------------------- //

QString TypeInfo::toString()
{
	return QString("%1\t%2\t%3\t%4\t").arg(id).arg(count).arg(name).arg(qualifiedName);
}

void TypeInfo::fromString( QString arg )
{
	id = arg.section("\t",0,0).toInt();
	count = arg.section("\t",1,1).toInt();
	name = arg.section("\t",2,2);
	qualifiedName = arg.section("\t",3,3);
}

