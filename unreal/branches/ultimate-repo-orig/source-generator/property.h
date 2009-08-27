#pragma once

#include <QDomElement>

class Entity;

class RealProperty {
	Entity *entity;
	QString description; // metametamodel wants it
	QString name;
	QString type;
	QString default_value;
	bool is_enum;
	bool is_ref;

public:
	RealProperty(Entity *e){entity = e;}
	~RealProperty(){}

	bool init(QDomElement &);
	bool isRef(){ return is_ref; }
	QString getName() {return name;}
	Entity* getOwner() {return entity;}
};
