#pragma once

#include <QList>
#include <QDomNode>

#include "non_graph_type.h"
#include "entity.h"

class EditorFile;

#define FOR_ALL_OBJECTS(c,o) \
	for (QList<Entity *>::ConstIterator o = c->constObjBegin(); \
		 o != c->constObjEnd(); o++)

class Editor{
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
private:
	EditorFile *efile;
	QMap<QString, QStringList> enumerations;
	QList<NonGraphType *> types_ng;
	QList<Entity *> objects;
	bool parseNonGraphTypes(QDomElement &);
	bool parseGraphTypes(QDomElement &);
	QString name;
};
