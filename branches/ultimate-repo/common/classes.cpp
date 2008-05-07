#include "classes.h"

void Root::addObject( int id, Object* obj )
{
	objects[id] = obj;
}
	
void Root::addLink( int id, Link* link )
{
	links[id] = link;
}

int Root::getObjectsSize()
{
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
   if( obj && obj->getType() == type ){
     res += QString("%1\t").arg(obj->getId());
   }
 }
 if( !res.isEmpty() )
   res.chop(1);
 else
   res = "\t";
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
 else
   res = "\t";
return res;
}

// --------------------------------------------------------------------------- //

Object::Object( int _id, int _type, int _x, int _y ) 
{
	id = _id;
	x = _x;
	y = _y;
	type = _type;
}

int Object::getX() 
{
	return x;
}	

void Object::setY( int arg )
{
	y = arg;
}

void Object::setX( int arg ) 
{
	x = arg;
}	

int Object::getY()
{
	return y;
}

void Object::setName( QString arg )
{
	name = arg;
}

QString Object::getName()
{
	return name;
}

void Object::setParent( int id )
{
	parent  = id;
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
	return children.count();
}

void Object::addChild( int child )
{
	if( !children.contains(child) )
		children << child;
}

QString Object::childrenToString()
{
	QString res = "";
	for( int i=0; i<children.size(); i++ )
		res += QString("%1\t").arg(children[i]);
		
	return res;	
}

QString Object::getConfiguration()
{
	return configuration;
}

void Object::setConfiguration( QString arg )
{
	configuration = arg;
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
	res += QString("%1\t%2\t%3\t%4\t%5\t%6\t%7\t%8\t")
			.arg(type).arg(id).arg(parent).arg(name).arg(configuration).arg(x).arg(y).arg(description);
	
	res += QString("%1\t").arg(children.size());
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


// --------------------------------------------------------------------------- //

Link::Link( int _id, int _type ) 
{
	id = _id;
	type = _type;
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

void Link::setParent( int id )
{
	parent  = id;
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
	res += QString("%1\t%2\t%3\t%4\t%5\t").arg(type).arg(id).arg(parent).arg(name).arg(description);
	
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

QString Link::getConfiguration()
{
	return configuration;
}

void Link::setConfiguration( QString arg )
{
	configuration = arg;
}

QString Link::getPosition()
{
	return position;
}

void Link::setPosition( QString arg )
{
	position = arg;
}

int Link::getFrom()
{
	if( objectsFrom.size() > 0 )
		return objectsFrom.at(0);
	
	return -1;	
}

int Link::getTo()
{
	if( objectsFrom.size() > 0 )
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

