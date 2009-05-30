#ifndef CATEGORY_H
#define CATEGORY_H

#include <QList>
#include <QDomNode>

#include "entity.h"

class EditorFile;

#define FOR_ALL_OBJECTS(c,o) \
	for (QList<Entity *>::ConstIterator o = c->constObjBegin(); \
	     o != c->constObjEnd(); o++)

class Editor{
	EditorFile *efile;
	QMap<QString, QStringList> enumerations;
	QList<Entity *> objects;
	bool parseEnum(QDomElement &);
	bool parseNumeric(QDomElement &);
	bool parseString(QDomElement &);
	bool parseNGT(QDomElement &);
	bool parseGT(QDomElement &);
	QString name;

public:
	Editor(QString, EditorFile *);
	~Editor();

	bool init(QDomElement &);
	bool resolve(void);
	QString get_name(void) const {return name;}
	const EditorFile* get_editor(void) const {return efile;}
	const Entity* findEntityInTree(QString) const;
	const Entity* findEntity(QString) const;

	QList<Entity*>::ConstIterator constObjBegin() const {return objects.constBegin();}
	QList<Entity*>::ConstIterator constObjEnd() const {return objects.constEnd();}
};

#endif
