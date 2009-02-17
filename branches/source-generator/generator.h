/** @file generator.h
 * 	@brief Генератор классов используемых на диаграммах элементов
*/
#ifndef GENERATOR_H
#define GENERATOR_H

#include <QtXml>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QFileInfo>

#include "editor_file.h"
#include "entity.h"

class Category;

/** @brief Вспомогательная переменная для генерации файла ресурсов */
extern QString resources;

#define FOR_ALL_FILES(f) \
	for (QList<EditorFile *>::ConstIterator f = loaded_files.constBegin(); \
	     f != loaded_files.constEnd(); f++)

#define MEGA_FOR_ALL_OBJECTS(f,c,o) \
	FOR_ALL_FILES(f) FOR_ALL_CATEGORIES((*f), c) FOR_ALL_OBJECTS((*c), o)

#define MEGA_FOR_ALL_OBJECTS_COUNTER(f,c,o,i) \
	do { \
		int i = 0; \
		FOR_ALL_FILES(f) FOR_ALL_CATEGORIES((*f), c) FOR_ALL_OBJECTS((*c), o) {

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
	Generator();
	~Generator();
	/** @brief Обработать все входные файлов и сгенерировать редакторы */
	bool generate();
	bool loadFile(QString, const EditorFile **f = NULL);
	const Category* findCategory(QString) const;
	void setSrcDir(QString path) {srcdir = path; }

private:
	/** @brief Начальное значение идентификаторов сущностей */
	static const int NUM = 1;
	QList<EditorFile *> loaded_files;

	QString srcdir;

	const EditorFile* findFile(QFileInfo) const;

	/** @brief Сгенерировать перечисления ролей */
	void genEnums();
	/** @brief Сгенерировать информацию о типах элементов */
	void genTypes();
	/** @brief Сгенерировать классы элементов */
	void genClasses();
	/** @brief Сгенерировать фабрику элементов */
	void genFactory();
	/** @brief Сгенерировать вспомогательную информацию для репозитория */
	void genRealRepoInfo();
	/** @brief Сгенерировать вспомогательные средства для проверки корректности соединения элементов */
	void genEdgesFunction();

	/** @brief Получить номер сущности в списке
	 * 	@brief @return Номер сузности в списке
	 * */
	int position( QString arg /**< Идентификатор */);

	/** @brief Директория */
	QDir dir;
};

#endif
