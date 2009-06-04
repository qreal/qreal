#pragma once

#include <QFileInfo>
#include "editor.h"

#define FOR_ALL_EDITORS(f,c) \
	for (QList<Editor *>::ConstIterator c = f->constEdBegin(); \
	     c != f->constEdEnd(); c++)

class EditorFile {
	QList<const EditorFile *> includes;
	QList<Editor *> editors;
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
	const Editor* findEditor(QString) const;

	QList<Editor*>::ConstIterator constEdBegin() const {return editors.constBegin();}
	QList<Editor*>::ConstIterator constEdEnd() const {return editors.constEnd();}
};
