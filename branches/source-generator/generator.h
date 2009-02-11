/** @file generator.h
 * 	@brief Генератор классов используемых на диаграммах элементов
*/
#ifndef GENERATOR_H
#define GENERATOR_H

#include <QtXml>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QFile>
#include <QDir>

#include "entity.h"

class Category;

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
	bool work( QStringList );

private:
	/** @brief Начальное значение идентификаторов сущностей */
	static const int NUM = 1;

	/** @brief Обработать файл */
	bool parseFile( QString arg /**< Имя файла */);
	/** @brief Осуществить разбор описания перечисления */
	bool parseEnum( QDomNode arg /**< Узел XML-описания */);
	/** @brief Осуществить разбор описания элемента */
	bool parseNode( QDomNode arg /**< Узел XML-описания */);
	/** @brief Осуществить разбор описания связи */
	bool parseEdge( QDomNode arg /**< Узел XML-описания */);

	/** @brief Обработать описание наследования */
	bool parseGeneralizations ( Entity* ent, /**< Сущность */
								QDomNode arg /**< Узел XML-описания */);
	/** @brief Осуществить разбор описаний свойств */
	bool parseProperties      ( Entity* ent, /**< Сущность */
								QDomNode arg /**< Узел XML-описания */);
	/** @brief Осуществить разбор описаний ассоциаций */
	bool parseAssociations    ( Entity* ent, /**< Сущность */
								QDomNode arg, /**< Узел XML-описания */
								bool isNode = false /**< Является ли сущность элементом */
								);
	//void parseNodeAssociations( Entity*, QDomNode );
	/** @brief Осуществить разбор описания графического представления элемента */
	bool parseSdf             ( Entity* ent, /**< Сущность */
								QDomNode arg /**< Узел XML-описания */
								);
	/** @brief Осуществить разбор описаний портов */
	bool parsePorts           ( Node* node, /**< Элемент */
								QDomNode arg /**< Узел XML-описания */
								);
	/** @brief Осуществить разбор описаний надписей, параметризующих SVG */
	bool parseLabels          ( Entity* ent,  /**< Сущность */
								QDomNode arg /**< Узел XML-описания */
								);
	/** @brief Осуществить разбор описаний графического представления связей */
	bool parseEdgeGraphics    ( Edge* ed,   /**< Связь */
								QDomNode arg /**< Узел XML-описания */
								);

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

	/** @brief Выполнить все распространения */
	bool propagateAll();
	/** @brief Осуществить распространение свойств элементов */
	bool propagateProperties( Entity* arg /**< Сущность*/);
	/** @brief Осуществить распространение ассоциаций */
	bool propagateAssocs( Edge* arg /**< Связь */);
	/** @brief Осуществить распространение родительских элементов */
	bool propagateParents( Entity* arg /**< Сущность */);
//    void findChildren( Entity*, QString );

	/** @brief Найти сущность с заданным идентификатором
	 * 	@brief @return Сущность
	 * */
	Entity* find( QString id /**< Идентифткатор */);
	/** @brief Найти связь с заданным идентификатором
	 * 	@brief @return Связь
	 * */
	Edge* findEdge( QString id /**< Идентификатор */);

	/** @brief Получить номер сущности в списке
	 * 	@brief @return Номер сузности в списке
	 * */
	int position( QString arg /**< Идентификатор */);

	/** @brief Число анонимных ассоциаций */
	int untitled;
	/** @brief Число всех объектов */
	int objectsCount;

	/** @brief DOM-документ */
	QDomDocument* doc;

	/** @brief Список элементов */
	QList< Entity* > objects;
	/** @brief Список связей */
	QList< Edge* > edges;

	/** @brief Список диаграмм */
	QList < Category* > categories;

	/** @brief Ассоциативный массив перечислений */
	QMap<QString, QStringList> enumerations;

	/** @brief Вспомогательная переменная для генерации файла ресурсов */
	QString res;
	/** @brief Вспомогательная переменная для генерации файла ресурсов */
	QString resources;

	/** @brief Директория */
	QDir dir;
};

/** @class Category
 * 	@brief Описание диаграммы
 * */
class Category{
public:
	/** @brief Название диаграммы */
	QString name;
	/** @brief Список объектов на диаграмме */
	QList<int> objects;
};

#endif
