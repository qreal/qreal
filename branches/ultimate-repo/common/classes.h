#ifndef __CLASSES_H__
#define __CLASSES_H__

#include <QList>
#include <QString>
#include <QMap>
#include <QDebug>

class Link;
class Object;

/* root entity to handle all the others ( the parent of all `projects', 
 * `diagrams', `objects', `links' etc. ). for internal use only.
 */
class Root
{
public:
	// called while creating new objects
	void addObject( int id, Object* obj );
	void addLink( int id, Link* link );

	Object * getObject( int );
	Link * getLink( int );

	int getObjectsSize();
	int getLinksSize();

	// return list of objects of type `type'. 
	// (mostly for object explorer needs)
	QString getObjectsByType( int type );
	QString getLinksByType( int type );

	// returns some data, to be updated soon according to 
	// suggestions made in trac wiki
	QString getLinkData( int type, int id );
	QString getObjectData( int type, int id );

	
private:
	QMap<int, Object*> objects;
	QMap<int, Link*> links;
};

/* 
 * class for all node entities
 */

class Object
{
public:
	// TODO: remove ID from constructor and generate it by repo server itself
	Object( int _id, int _type, int _x, int _y );
	
	// TODO: return QPointF or something like that
	int getX();
	int getY();
	
	// TODO: add setPointF() method 
	void setX( int );
	void setY( int );

	QString getName();
	void setName( QString arg );
	
	void setProperty( QString name, QString val );
	QString getProperty( QString name );
	
	int getId();
	int getType();
	
	int childrenCount();

	// add child to the children list, nothing more
	void addChild( int );

	// returns list of children entities' IDs
	QString childrenToString();
	
	// set/get object's size configuration
	void setConfiguration( QString arg );
	QString getConfiguration();

	QString toString();
	
	QString getLinks();

	void addLink( int );
	void removeLink( int );

private:
	int x;
	int y;
	int id;
	int type;
	int parent;
	QString name;
	QString configuration;
	QList<int> children;
	QMap<QString, QString> props;
	QList<int> links;
};

/* class for all edge entities
 */

class Link
{
public:
	Link( int _id, int _type ); 

	int getId();
	int getType();
	QString getName();
	
	void setProperty( QString name, QString val );
	QString getProperty( QString name );

	void setName( QString arg );
	
	QString toString();
	
	QString getObjects();
	
	void addObjectTo( int );
	void addObjectFrom( int );

	// object's port position
	void setConfiguration( QString arg );
	QString getConfiguration();

	QString getPosition();
	void setPosition( QString );

private:
	QString getObjectsTo();
	QString getObjectsFrom();

	int id;
	int type;
	int parent;
	QString name;
	QString configuration;
	QString position;
	QMap<QString, QString> props;
	QList<int> objectsFrom;
	QList<int> objectsTo;
};

/* types description
 */

class TypeInfo{
public:
	int id;
	int count; // number of objects created ( for object explorer needs )
	QString name;
	QString qualifiedName; // `normal' description

	QString toString();
	void fromString( QString );
};

#endif //__CLASSES_H__
