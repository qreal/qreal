/** @file generator.h
 * 	@brief Генератор классов используемых на диаграммах элементов
*/
#pragma once

#include <QtXml>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QFileInfo>
#include "sdftocpp.h"
#include "editor_file.h"
#include "entity.h"

class Editor;

#define FOR_ALL_FILES(f) \
	for (QList<EditorFile *>::ConstIterator f = mLoadedFiles.constBegin(); \
		 f != mLoadedFiles.constEnd(); f++)

#define MEGA_FOR_ALL_OBJECTS(f,c,o) \
	FOR_ALL_FILES(f) FOR_ALL_EDITORS((*f), c) FOR_ALL_OBJECTS((*c), o)

#define MEGA_FOR_ALL_OBJECTS_COUNTER(f,c,o,i) \
	do { \
		int i = 0; \
		FOR_ALL_FILES(f) FOR_ALL_EDITORS((*f), c) FOR_ALL_OBJECTS((*c), o) {

#define MEGA_FOR_ALL_OBJECTS_COUNTER_END(i) \
			i++; \
		} \
	} while (0);

// the generator itself
/** @class Generator
 * 	@brief Генератор классов используемых на диаграммах элементов
*/
class Generator
{
public:
	Generator(QString const &inputXml);
	~Generator();
	/** @brief Обработать все входные файлы и сгенерировать редакторы */
	bool generate(QString const &outputFileName);
	bool loadFile(QString const &fileName, const EditorFile **f = NULL);
	const Editor* findEditor(QString const &editorName) const;
	void addResource(QString const &resourceXml);

private:
	/** @brief Начальное значение идентификаторов сущностей */
	static const int NUM = 1;

	void setSrcDir(QString const &path) { mSrcDir = path; }
	const EditorFile* findFile(QFileInfo const &file) const;

	void genPluginHeader(QString const &plugin);
	void genPluginSource(QString const &plugin);
	void genElementClasses(QString const &plugin);
	void genNodeClass(Node* node, QString const &plugin);
	void genEdgeClass(Edge* edge, QString const &plugin);
	void genProFile(QString const &plugin) const;

	QString normalizeName(QString const &name) const;
	QString upperFirst(const QString &str) const;

	void reportViciousCircle(QFileInfo const &fileInfo) const;
	static void printFiles(QStringList const &files, QTextStream &out);

	/** @brief Директория */
	QDir mDir;
	QString mSrcDir;

	bool mRecursive;
	QStringList mSources;
	QStringList mHeaders;
	QList<EditorFile *> mLoadedFiles;
	QString mResources;
};
