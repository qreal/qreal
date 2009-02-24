#ifndef PROPERTY_H
#define PROPERTY_H

#include <QDomElement>

class Entity;

class RealProperty {
	Entity *entity;
	QString description; // metametamodel wants it
	QString name;
	QString type;
	QString default_value;
	bool is_enum;

public:
	RealProperty(Entity *e){entity = e;}
	~RealProperty(){}

	bool init(QDomElement &);
	QString getName() {return name;}
	Entity* getOwner() {return entity;}
};

#endif
