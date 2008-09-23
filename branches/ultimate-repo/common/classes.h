/** @file classes.h
 * 	@brief Классы, используемые для хранения элементов диаграмм в репозитории
 * */

#ifndef __CLASSES_H__
#define __CLASSES_H__

#include <QList>
#include <QString>
#include <QMap>
#include <QDebug>

#include "defs.h"

class Link;
class Object;

/* root entity to handle all the others ( the parent of all `projects', 
 * `diagrams', `objects', `links' etc. ). for internal use only.
 */
/** @class Root
 * 	@brief Корневой элемент, управляет хранением всех остальных
 * */
class Root
{
public:
	~Root();

	// called while creating new objects
	/** @brief Добавить объект */
	void addObject( int id, /**< Идентификатор */
					Object* obj /**< Объект */
					);
	/** @brief Добавить связь */
	void addLink( int id, /**< Идентификатор */
				Link* link /**< Связь */
				);
	/** @brief Удалить объект */
	void deleteObject( int id /**< Идентификатор */);
	/** @brief Удалить связь */
	void deleteLink( int id /**< Идентификатор */);

	/** @brief Получить объект 
	 * 	@brief @return Объект
	 * */
	Object * getObject( int arg /**< Идентификатор */);
	/** @brief Получить связь 
	 * 	@brief @return Связь
	 * */
	Link * getLink( int arg /**< Идентификатор */);

	/** @brief Получить число объектов */
	int getObjectsSize();
	/** @brief Получить число связей */
	int getLinksSize();

	// return list of objects of type `type'. 
	// (mostly for object explorer needs)
	/** @brief Получить список идентификаторов объектов данного типа 
	 * 	@brief @return Список идентификаторов объектов данного типа
	 * */
	QString getObjectsByType( int type /**< Идентификатор типа */);
	/** @brief Получить список идентификаторов связей данного типа 
	 *	@brief @return Список идентификаторов связей данного типа
	 * */
	QString getLinksByType( int type /**< Идентификатор типа */);

	// returns some data, to be updated soon according to 
	// suggestions made in trac wiki
	/** @brief Получить данные об объекте 
	 *	@brief @return Данные об объекте
	 * */
	QString getLinkData( int type, /**< Идентификатор типа */
						int id /**< Идентификатор связи */
						);
	/** @brief Получить данные о связи 
	 *	@brief @return Данные о связи
	 * */
	QString getObjectData( int type, /**< Идентификатор типа */
							int id /**< Идентификатор объекта */
							);

	
//private:
	/** @brief Объекты */
	QMap<int, Object*> objects;
	/** @brief Связи */
	QMap<int, Link*> links;
};

/* 
 * class for all node entities
 */
/** @class Object
 * 	@brief Класс для хранения информации об объектах
 * */
class Object
{
public:
	// TODO: remove ID from constructor and generate it by repo server itself
	Object( int _id, int _type, int _x, int _y );
	
	// TODO: return QPointF or something like that
	/** @brief Получить координату Х объекта
	 *	@brief @return Координата Х
	 * */
	int getX();
	/** @brief Получить координату Y объекта
	 *	@brief @return Координата Y
	 * */
	int getY();
	
	// TODO: add setPointF() method 
	/** @brief Устновить координату X объекта */
	void setX( int x /**< Координата X*/);
	/** @brief Установить координату Y объекта */
	void setY( int y /**< Координата Y*/);

	/** @brief Получить имя объекта 
	 *	@brief @return Имя объекта
	 * */
	QString getName();
	/** @brief Установить имя объекта */
	void setName( QString arg /**< Имя */);

	/** @brief Получить идентификатор родителя объекта 
	 *	@brief @return Идентификатор родитля 
	 * */
	int getParent();
	/** @brief Установить идентификатор родителя объекта */
	void setParent( int id /**< Идентификатор родительского элемента */);

	/** @brief Получить описание объекта
	 *	@brief @return Описание объекта 
	 * */
	QString getDescription();
	/** @brief Установить описание объекта */
	void setDescription( QString desc /**< Описание */);
	
	/** @brief Установить значние свойства */
	void setProperty( QString name, /**< Свойство */
					QString val /**< Значение */
					);
	/** @brief Получить значение свойства 
	 *	@brief @return Значение свойства 
	 * */
	QString getProperty( QString name /**< Свойство */);
	
	/** @brief Получить идентификатор объекьа 
	 *	@brief @return Идентификатор объекта
	 * */
	int getId();
	/** @brief Получить тип объекта 
	 *	@brief @return Тип объекта 
	 * */
	int getType();
	
	/** @brief Получить число детей объекта 
	 *	@brief @return Число детей объекта
	 * */
	int childrenCount();

	// add child to the children list, nothing more
	/** @brief Добавить дочерний элемент */
	void addChild( int id /**< Идентификатор элемента */);
	/** @brief Убрать элемент из списка дочерних */
	void removeChild( int id /**< Идентификатор элемента*/);

	// returns list of children entities' IDs
	/** @brief Получить список идентификаторов дочерних элементов */
	QString childrenToString();
	
	// set/get object's size configuration
	/** @brief Установить конфигурацию объекта */
	void setConfiguration( QString arg /**< Конфигурация*/);
	/** @brief Получить конфигурацию объекта 
	 *	@brief @return Конфигурация объекта 
	 * */
	QString getConfiguration();

	/** @brief Сериализовать объект в строку
	 *	@brief @return Сериализованный объект
	 * */
	QString toString();
	/** @brief Получить список идентификаторо входящих связей 
	 *	@brief @return Список идентификаторов входящих связей
	 * */
	QString getIncomingLinks();
	/** @brief Получить список идентификаторов исходящих связей
	 *	@brief @return Список идентификаторов исходящих связей
	 * */
	QString getOutcomingLinks();
	
	/** @brief Добавить связь */
	void addLink( int id, /**< Идентификатор связи */
				int die /**< Направленность связи */
				);
	/** @brief Убрать связь */
	void removeLink( int id, /**< Идентификатор связи */
					int dir /**< Направленность связи */
					);
	
	/** @brief Отладочный вывод */
	void print(){
		qDebug() << "incoming :" << incomingLinks;
		qDebug() << "outcoming:" << outcomingLinks;
		qDebug() << "props:" <<  props;
	}

private:
	/** @brief Координата X */
	int x;
	/** @brief Координата Y */
	int y;
	/** @brief Идентификатор */
	int id;
	/** @brief Тип */
	int type;
	/** @brief Родительский элемент */
	int parent;
	/** @brief Имя */
	QString name;
	/** @brief Описание */
	QString description;
	/** @brief Конфигурация */
	QString configuration;
	/** @brief Список дочерних элементов */
	QList<int> children;
	/** @brief Свойства и их значения */
	QMap<QString, QString> props;
	/** @brief Список входящих связей */
	QList<int> incomingLinks;
	/** @brief Список исходящий связей */
	QList<int> outcomingLinks;
};

/* class for all edge entities
 */
/** @class Link
 * 	@brief Класс для хранения информации о связях
 * 	*/
class Link
{
public:
	Link( int _id, int _type ); 
	
	/** @brief Получить идентификатор связи
	 * 	@brief @return Идентификатор связи
	 * 	*/
	int getId();
	/** @brief Получить тип связи 
	 * 	@brief @return Тип связи
	 * 	*/
	int getType();
	
	/** @brief Получить имя связи 
	 * 	@brief @return Имя связи
	 * 	*/
	QString getName();
	/** @brief Установить имя связи */
	void setName( QString arg /**< Имя */);
	
	/** @brief Получить идентификатор родительского элемента
	 * 	@brief @return Идентификатор родительского элемента 
	 * 	*/
	int getParent();
	/** @brief Установить идентификатор родительского элемента */
	void setParent( int id /**< Идентификатор родительского элемента */);
	
	/** @brief Получить описание связи 
	 * 	@brief @return Описание связи 
	 * 	*/
	QString getDescription();
	/** @brief Установить описание связи */
	void setDescription( QString /**< Описание */);

	/** @brief Установить значние свойства */
	void setProperty( QString name, /**< Свойство */
						QString val /**< Значение свойства */
						);
	/** @brief Получить значение свойства 
	 * 	@brief @return Значение свойства
	 * 	*/
	QString getProperty( QString name /**< Свойство */);

	/** @brief Сериализовать связь в строку
	 * 	@brief @return Строковое представление связи 
	 * 	*/
	QString toString();
	
	/** @brief Получить список объектов, к которым присоединена связь 
	 * 	@brief @return Список объектов, к которым присоединена связь
	 * 	*/
	QString getObjects();
	
	/** @brief Присоединить объект к началу связи */
	void addObjectTo( int id /**< Идентификатор объекта */);
	/** @brief Присоединить объект к концу связи */
	void addObjectFrom( int id /**< Идентификатор объекта */);

	/** @brief Отсоединить объект от начала связи */
	void removeObjectTo( int id /**< Идентификатор объекта */);
	/** @brief Отсоединить объект от конца связи */
	void removeObjectFrom( int id /**< Идентификатор объекта */);

	// object's port position
	/** @brief Установить конфигурацию связи*/
	void setConfiguration( QString arg /**< Конфигурация */);
	/** @brief Получить конфигурацию связи 
	 * 	@brief @return Конфигурация связи
	 * 	*/
	QString getConfiguration();

	/** @brief Получить расположение связи
	 * 	@brief @return Расположение связи
	 * 	*/
	QString getPosition();
	/** @brief Установить расположение связи */
	void setPosition( QString arg /**< Расположение связи */);

	/** @brief Получить идентификатор объекта, присоединенного к началу связи 
	 * 	@brief @return Идентификатор объекта, присоединенного к началу связи
	 * 	*/
	int getFrom();
	/** @brief Получить идентификатор объекта, присоединенного к концу связи
	 * 	@brief @return Идентификатор объекта, присоединенного к концу связи
	 * 	*/
	int getTo();
	
	/** @brief Отладочная печать */
	void print(){
		qDebug() << "to: :" << objectsTo;
		qDebug() << "from:" << objectsFrom;
		qDebug() << "props:" <<  props;
	}


private:
	/** @brief Получить список идентификаторов объектов, присоединенных к концу связи */
	QString getObjectsTo();
	/** @brief Получить список идентификаторов объектов, присоединенных к качалу связи */
	QString getObjectsFrom();

	/** @brief Идентификатор */
	int id;
	/** @brief Тип */
	int type;
	/** @brief Родительский элемент */
	int parent;
	/** @brief Имя */
	QString name;
	/** @brief Описание */
	QString description;
	/** @brief Конфигурация */
	QString configuration;
	/** @brief Расположение */
	QString position;
	/** @brief Свойства и их значения */
	QMap<QString, QString> props;
	/** @brief Объекты, присоединенные к концу связи */
	QList<int> objectsTo;
	/** @brief Объекты, присоединенные к началу связи */
	QList<int> objectsFrom;
};

/* types description
 */
/** @class TypeInfo
 * 	@brief Описание типа 
 * */
class TypeInfo{
public:
	/** @brief Идентификатор */
	int id;
	/** @brief Число объектов данного типа */
	int count; // number of objects created ( for object explorer needs )
	/** @brief Имя */
	QString name;
	/** @brief Описание */
	QString qualifiedName; // `normal' description

	/** @brief Сериализовать описание типа в строку
	 * 	@brief @return Строковое представление информации о типе
	 * 	*/
	QString toString();
	/** @brief Десериализовать описание типа */
	void fromString( QString arg /**< Сериализованное значение описания типа */);
};

#endif //__CLASSES_H__
