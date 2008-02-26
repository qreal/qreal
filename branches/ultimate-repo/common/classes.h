#ifndef __CLASSES_H__
#define __CLASSES_H__

#include <QList>
#include <QString>
#include <QMap>
#include <QDebug>

class Link;
class Object;

class Root 
{
public:
	void addObject( int id, Object* obj );
	void addLink( int id, Link* link );

	Object * getObject( int );
	Link * getLink( int );

	int getObjectsSize();
	int getLinksSize();

	QString getObjectsByType( int type );
	QString getLinksByType( int type );

	QString getLinkData( int type, int id );
	QString getObjectData( int type, int id );

	
private:
	QMap<int, Object*> objects;
	QMap<int, Link*> links;
};

class Object
{
public:
	Object( int _id, int _type, int _x, int _y );

	int getX();
	int getY();
	
	void setX( int );
	void setY( int );

	QString getName();
	void setName( QString arg );

	void setProperty( QString name, QString val );
	QString getProperty( QString name );
	
	int getId();
	int getType();
	
	int childrenCount();

	void addChild( int );

	QString childrenToString();
	
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

class TypeInfo{
public:
	int id;
	int count;
	QString name;
	QString qualifiedName;

	QString toString();
	void fromString( QString );
};

#endif //__CLASSES_H__
