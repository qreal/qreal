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
		if( objects.values().value(i) && objects.values().value(i)->getType() == type ){
			res += QString("%1\t").arg(objects.values().value(i)->getId());
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
	qDebug() << "size is " << links.size();
	for( int i=0; i<links.size(); i++ ){
		if( links.value(i) && links.value(i)->getType() == type )
			res += QString("%1\t").arg(links.value(i)->getId());
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

QString Object::getLinks()
{
	QString res = "";
	for( int i=0; i<links.size(); i++)		
		res += QString("%1\t").arg(links.at(i));
	return res;
}

QString Object::toString()
{
	QString res = "";
	res += QString("%1\t%2\t%3\t%4\t%5\t%6\t%7\t")
			.arg(type).arg(id).arg(parent).arg(name).arg(configuration).arg(x).arg(y);
	
	res += QString("%1\t").arg(children.size());
	res += childrenToString();

	res += QString("%1\t").arg(links.size());
	res += getLinks();

	res += QString("%1\t").arg(props.size());
	for( int i=0; i<props.keys().size(); i++){
		res += QString("%1;%2\t").arg(props.keys().at(i)).arg(props.value(props.keys().at(i)));
	}
	return res;
}

void Object::addLink( int id )
{
	if( !links.contains(id) )
		links << id;
}

void Object::removeLink( int id )
{
	links.removeAll(id);
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
	res += QString("%1\t%2\t%3\t%4\t").arg(type).arg(id).arg(parent).arg(name);
	
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

