/** @file classes.cpp
 * 	@brief Классы, используемые для хранения элементов диаграмм в репозитории
 * */
#include "classes.h"

Root::~Root()
{
	foreach( Object *obj, objects.values() )
		delete obj;
	foreach( Link *link, links.values() )
		delete link;
	objects.clear();
	links.clear();
}

void Root::addObject( int id, Object* obj )
{
	obj->addRef();
	objects[id] = obj;
}

void Root::addLink( int id, Link* link )
{
	link->addRef();
	links[id] = link;
}

void Root::deleteObject( int id )
{
	objects.remove(id);
}

void Root::deleteLink( int id )
{
	objects.remove(id);
}

int Root::getObjectsSize()
{
	foreach( Object* obj, objects)
		qDebug() << "id: " << obj->getId() << ", name: " << obj->getName();
	return objects.size();
}

int Root::getLinksSize()
{
	return links.size();
}

Object *Root::getObject( int id )
{
	if( objects.contains(id))
		return objects[id];
	else
		return 0;
}

Link *Root::getLink( int id )
{
	if( links.contains(id) )
		return links[id];
	else
		return 0;
}

QString Root::getObjectsByType( int type )
{
	QString res = "";
	for( int i=0; i<objects.values().size(); i++ ){
		int key = objects.keys()[i];
		Object *obj = objects.value(key);
		if( obj && obj->getType() == type )
			res += QString("%1\t").arg(obj->getId());
	}
	if( !res.isEmpty() )
		res.chop(1);
	return res;
}

QString Root::getLinksByType( int type )
{
	QString res = "";
	for( int i=0; i<links.size(); i++ ){
		int key = links.keys()[i];
		Link *link = links.value(key);
		if( link && link->getType() == type )
			res += QString("%1\t").arg(link->getId());
	}

	if( !res.isEmpty() )
		res.chop(1);
	return res;
}

// --------------------------------------------------------------------------- //

Object::Object( int _id, int _type )
{
	id = _id;
	type = _type;
	refs = 0;
}

void Object::setName( QString arg )
{
	name = arg;
}

QString Object::getName()
{
	return name;
}

void Object::setDescription( QString arg )
{
	description = arg;
}

QString Object::getDescription()
{
	return description;
}

int Object::getId()
{
	return id;
}

int Object::getType()
{
	return type;
}

int Object::childrenCount()
{
	return nodeChildren.count() + edgeChildren.count();
}

void Object::addNodeChild( int child )
{
	NodeOnDiagram node(child);
	nodeChildren[child] = node;
}

void Object::addEdgeChild( int child )
{
	EdgeOnDiagram edge(child);
	edgeChildren[child] = edge;
}

void Object::removeNodeChild( int child )
{
	foreach( NodeOnDiagram node, nodeChildren)
		if( node.getId() == child ){
			nodeChildren.remove(child);
//			qDebug() << "removed node with id " << child << " from diagram " << this->id;
		}
}

void Object::removeEdgeChild( int child )
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

void Object::setProperty( QString name, QString val )
{
	props[name] = val;
}

QString Object::getProperty( QString name )
{
	return props.value(name);
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

void Object::addLink( int id, int dir )
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

void Object::removeLink( int id, int dir )
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

QString Object::getChildPos( int id )
{
	if( edgeChildren.contains(id) )
		return edgeChildren[id].getPosition();
	else
		return "";
}

bool Object::setChildPos( int id, QString pos )
{
	if( edgeChildren.contains(id) )
		edgeChildren[id].setPosition(pos);
	else
		return false;
	return true;
}

QPoint Object::getChildCoord( int id)
{
	if( nodeChildren.contains(id) )
		return nodeChildren[id].getCoord();
	return QPoint();
}

bool Object::setChildCoord( int id, QPoint p)
{
	if( nodeChildren.contains(id) )
		nodeChildren[id].setCoord(p);
	else
		return false;
	return true;
}

QString Object::getChildConfiguration( int id)
{
	if( nodeChildren.contains(id) )
		return nodeChildren[id].getConfiguration();
	else if ( edgeChildren.contains(id) )
		return edgeChildren[id].getConfiguration();
	else
		return "";
}

bool Object::setChildConfiguration( int id, QString conf )
{
	if( nodeChildren.contains(id) )
		nodeChildren[id].setConfiguration(conf);
	else if( edgeChildren.contains(id) )
		edgeChildren[id].setConfiguration(conf);
	else
		return false;
	return true;
}

// --------------------------------------------------------------------------- //

Link::Link( int _id, int _type )
{
	id = _id;
	type = _type;
	refs = 0;
}

int Link::getId()
{
	return id;
}

int Link::getType()
{
	return type;
}

QString Link::getName()
{
	return name;
}

void Link::setName( QString arg )
{
	name = arg;
}

void Link::setDescription( QString arg )
{
	description = arg;
}

QString Link::getDescription()
{
	return description;
}

void Link::setProperty( QString name, QString val )
{
	props[name] = val;
}

QString Link::getProperty( QString name )
{
	return props.value(name);
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

void Link::addObjectTo( int id )
{
	if( !objectsTo.contains( id ) )
		objectsTo << id;
}

void Link::addObjectFrom( int id )
{
	if( !objectsFrom.contains( id ) )
		objectsFrom << id;
}

void Link::removeObjectTo( int id )
{
	objectsTo.removeAll(id);
}

void Link::removeObjectFrom( int id )
{
	objectsFrom.removeAll(id);
}

int Link::getFrom()
{
	if( objectsFrom.size() > 0 )
		return objectsFrom.at(0);

	return -1;
}

int Link::getTo()
{
	if( objectsTo.size() > 0 )
		return objectsTo.at(0);

	return -1;
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

