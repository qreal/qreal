#ifndef EDITOR_FILE_H
#define EDITOR_FILE_H

#include "category.h"

#define FOR_ALL_CATEGORIES(f,c) \
	for (QList<Category *>::ConstIterator c = f->constCatBegin(); \
	     c != f->constCatEnd(); c++)

class EditorFile {
	QList<EditorFile *> includes;
	QList<Category *> categories;
	QString name;
	QString uniq_name;
	Generator *generator;
	bool loading_done;

public:
	EditorFile(QString, Generator*);
	~EditorFile();

	bool load(void);
	bool isLoaded(void) const {return loading_done;};
	QString get_name(void) const {return name;}
	QString get_uniq_name(void) const {return uniq_name;}
	const Entity* findEntityInIncludes(QString) const;
	const Entity* findEntityInCategories(QString) const;
	const Category* findCategory(QString) const;

	QList<Category*>::ConstIterator constCatBegin() const {return categories.constBegin();}
	QList<Category*>::ConstIterator constCatEnd() const {return categories.constEnd();}
};

#endif
