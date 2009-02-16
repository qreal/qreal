#ifndef CATEGORY_H
#define CATEGORY_H

#include <QList>
#include <QDomNode>

#include "entity.h"

class EditorFile;

#define FOR_ALL_OBJECTS(c,o) \
	for (QList<Entity *>::ConstIterator o = c->objects.constBegin(); \
	     o != c->objects.constEnd(); o++)

class Category{
	EditorFile *efile;
	QMap<QString, QStringList> enumerations;
	bool parseEnum(QDomElement &);
	QString name;

public:
	Category(QString, EditorFile *);
	~Category();

	// Hack
	QList<Entity *> objects;

	bool init(QDomElement &);
	bool resolve(void);
	QString get_name(void) const {return name;}
	const EditorFile* get_editor(void) const {return efile;}
	const Entity* findEntityInTree(QString) const;
	const Entity* findEntity(QString) const;
};

#endif
