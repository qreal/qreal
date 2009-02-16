#ifndef EDITOR_FILE_H
#define EDITOR_FILE_H

#include "category.h"

#define FOR_ALL_CATEGORIES(f,c) \
	for (QList<Category *>::ConstIterator c = f->categories.constBegin(); \
	     c != f->categories.constEnd(); c++)

class EditorFile {
	QList<EditorFile *> includes;
	QString name;
	QString uniq_name;
	Generator *generator;
	bool loading_done;

public:
	EditorFile(QString, Generator*);
	~EditorFile();

	// Hack
	QList<Category *> categories;

	bool load(void);
	bool isLoaded(void) const {return loading_done;};
	QString get_name(void) const {return name;}
	QString get_uniq_name(void) const {return uniq_name;}
	const Entity* findEntityInIncludes(QString) const;
	const Entity* findEntityInCategories(QString) const;
	const Category* findCategory(QString) const;
};

#endif
