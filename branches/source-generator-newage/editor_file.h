#ifndef EDITOR_FILE_H
#define EDITOR_FILE_H

#include <QFileInfo>
#include "category.h"

#define FOR_ALL_CATEGORIES(f,c) \
	for (QList<Category *>::ConstIterator c = f->constCatBegin(); \
	     c != f->constCatEnd(); c++)

class EditorFile {
	QList<const EditorFile *> includes;
	QList<Category *> categories;
	QFileInfo fileinfo;
	Generator *generator;
	bool loading_done;

public:
	EditorFile(QFileInfo, Generator*);
	~EditorFile();

	bool load(void);
	bool isLoaded(void) const {return loading_done;};
	QString fullPath(void) const {return fileinfo.canonicalFilePath();}
	QFileInfo fileInfo(void) const {return fileinfo;}
	const Entity* findEntityInIncludes(QString) const;
	const Entity* findEntityInCategories(QString) const;
	const Category* findCategory(QString) const;

	QList<Category*>::ConstIterator constCatBegin() const {return categories.constBegin();}
	QList<Category*>::ConstIterator constCatEnd() const {return categories.constEnd();}
};

#endif
