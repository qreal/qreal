/** @file classes.h
 * 	@brief Классы, используемые для хранения элементов диаграмм в репозитории
 * */

#pragma once

#include <QList>
#include <QString>
#include <QMap>
#include <QDebug>
#include <QPoint>

#include "defs.h"

class Link;
class Object;
class NodeOnDiagram;
class EdgeOnDiagram;

typedef QString IdType;
typedef QString TypeIdType;

/** @class RepoData
 * 	@brief Класс, предоставляющий доступ ко всем объектам в репозитории.
 * */
class RepoData
{
public:
	RepoData();
	~RepoData();

	// called while creating new objects
	/** @brief Добавить объект */
	void addObject( IdType const &id, /**< Идентификатор */
					Object* obj /**< Объект */
					);
	/** @brief Добавить связь */
	void addLink( IdType const &id, /**< Идентификатор */
				Link* link /**< Связь */
				);
	/** @brief Удалить объект */
	void deleteObject( IdType const &id /**< Идентификатор */);
	/** @brief Удалить связь */
	void deleteLink( IdType const &id /**< Идентификатор */);

	/** @brief Получить объект
	 * 	@brief @return Объект
	 * */
	Object * getObject( IdType const &id/**< Идентификатор */);
	/** @brief Получить связь
	 * 	@brief @return Связь
	 * */
	Link * getLink( IdType const &id/**< Идентификатор */);

	/** @brief Получить число объектов */
	int getObjectsSize();
	/** @brief Получить число связей */
	int getLinksSize();

	// return list of objects of type `type'.
	// (mostly for object explorer needs)
	/** @brief Получить список идентификаторов объектов данного типа
	 * 	@brief @return Список идентификаторов объектов данного типа
	 * */
	QString getObjectsByType( TypeIdType const &type /**< Идентификатор типа */);
	/** @brief Получить список идентификаторов связей данного типа
	 *	@brief @return Список идентификаторов связей данного типа
	 * */
	QString getLinksByType( TypeIdType const &type /**< Идентификатор типа */);

	// returns some data, to be updated soon according to
	// suggestions made in trac wiki
	/** @brief Получить данные об объекте
	 *	@brief @return Данные об объекте
	 * */
	QString getLinkData( TypeIdType const &type, /**< Идентификатор типа */
						IdType const &id /**< Идентификатор связи */
						);
	/** @brief Получить данные о связи
	 *	@brief @return Данные о связи
	 * */
	QString getObjectData( TypeIdType const &type, /**< Идентификатор типа */
							IdType const &id /**< Идентификатор объекта */
							);
	/** @brief Получить корневой объект репозитория
	 *	@brief @return Корневой объект репозитория. (с) Капитан Очевидность.
	 * */
	Object* getRootObject() const;

	/** @brief Получить все объекты в репозитории. */
	QList<IdType> getAllObjects() const;
	/** @brief Получить все линки в репозитории. */
	QList<IdType> getAllLinks() const;

	/** @brief Удалить всё из репозитория. */
	void clearAll();
private:
	/** @brief Объекты */
	QMap<IdType, Object*> objects;
	/** @brief Связи */
	QMap<IdType, Link*> links;
	/** @brief Корневой объект репозитория */
	Object *mRoot;
};

/** @class RepoElement
 * 	@brief Базовый класс для объектов и линков в репозитории
 * */
class RepoElement
{
public:
	// TODO: remove ID from constructor and generate it by repo server itself
	RepoElement(IdType const &id, TypeIdType const &type);
	virtual ~RepoElement(){}

	/** @brief Получить имя элемента
	 *	@brief @return Имя элемента
	 * */
	QString getName();
	/** @brief Установить имя элемента */
	void setName( QString name/**< Имя */);

	/** @brief Получить описание элемента
	 *	@brief @return Описание элемента
	 * */
	QString getDescription();
	/** @brief Установить описание элемента */
	void setDescription( QString desc /**< Описание */);

	/** @brief Установить значние свойства */
	void setProperty( QString name, /**< Свойство */
					QString val /**< Значение */
					);
	/** @brief Получить значение свойства
	 *	@brief @return Значение свойства
	 * */
	QString getProperty( QString name /**< Свойство */);

	/** @brief Получить идентификатор элемента
	 *	@brief @return Идентификатор элемента
	 * */
	IdType getId();
	void setId(IdType const &id);
	/** @brief Получить тип элемента
	 *	@brief @return Тип элемента
	 * */
	TypeIdType getType();

	/** @brief Получить список идентификаторов родителей */
	QString parentsToString();

	/** @brief Сериализовать сущность в строку
	 *	@brief @return Сериализованный объект
	 * */
	virtual QString toString() = 0;

	virtual QString getReferrals() { return QString("");}

	void addRef(IdType const &parent)
	{
		if (!parents.contains(parent))
			parents.append(parent);
	}

	void removeAllRefs()
	{
		parents.clear();
	}

	void removeRef(IdType const &parent)
	{
		if (parents.contains(parent)) {
			parents.removeAll(parent);
		}
	}

	int refCount()
	{
		return parents.count();
	}

protected:
	/** @brief Идентификатор */
	IdType id;
	/** @brief Тип */
	TypeIdType type;
	/** @brief Имя */
	QString name;
	/** @brief Описание */
	QString description;
	/** @brief Свойства и их значения */
	QMap<QString, QString> props;
	/** @brief Список родителей элемента */
	QList<IdType> parents;
};

/** @class Object
 * 	@brief Класс для хранения информации об объектах
 * */
class Object: public RepoElement
{
public:
	Object( IdType const &id, TypeIdType const &type );

	/** @brief Получить число детей объекта
	 *	@brief @return Число детей объекта
	 * */
	int childrenCount();

	// add child to the children list, nothing more
	/** @brief Добавить дочерний элемент-объект */
	void addNodeChild( IdType const &id /**< Идентификатор элемента */);
	/** @brief Добавить дочерний элемент-связь */
	void addEdgeChild( IdType const &id /**< Идентификатор элемента */);
	/** @brief Убрать элемент из списка дочерних */
	void removeNodeChild( IdType const &id /**< Идентификатор элемента*/);
	/** @brief Убрать связь из списка дочерних */
	void removeEdgeChild( IdType const &id /**< Идентификатор связи*/);

	/** @brief Проверить, является ли указанный объект сыном данного */
	bool isParentOf(IdType const &id /**< Идентификатор элемента*/) const;

	/** @brief Изменить расположение дочернего элемента
	 * 	@brief @return Успешность выполнения операции
	 * */
	bool setChildPos( IdType const &id, /**< Идентификатор элемента */
					QString pos /**< Расположение элемента */
					);
	/** @brief Изменить конфигурацию дочернего элемента
	 * 	@brief @return Успешность выполнения операции
	 * */
	bool setChildConfiguration( IdType const &id, /**< Идентификатор элемента */
							QString conf /**< Конфигурация элемента */
							);
	/** @brief Изменить координаты дочернего элемента
	 * 	@brief @return Успешность выполнения операции
	 * */
	bool setChildCoord( IdType const &id, /**< Идентификатор элемента */
						QPoint p /**< Координаты элемента */
						);
	/** @brief Получить расположение дочернего элемента
	 * 	@brief @return Расположение дочернего элемента
	 * */
	QString getChildPos( IdType const &id /**< Идентификатор элемента */);
	/** @brief Получить конфигурацию дочернего элемента
	 * 	@brief @return Конфигурация дочернего элемента
	 * */
	QString getChildConfiguration( IdType const &id /**< Идентификатор элемента */);
	/** @brief Получить координаты дочернего элемента
	 * 	@brief @return Координаты дочернего элемента
	 * 	* */
	QPoint getChildCoord( IdType const &id /**< Идентификатор элемента */);

	// returns list of children entities' IDs
	/** @brief Получить список идентификаторов дочерних элементов */
	QString childrenToString();

	/** @brief Сериализовать объект в строку
	 *	@brief @return Сериализованный объект
	 * */
	virtual QString toString();
	/** @brief Получить список идентификаторо входящих связей
	 *	@brief @return Список идентификаторов входящих связей
	 * */
	QString getIncomingLinks();
	/** @brief Получить список идентификаторов исходящих связей
	 *	@brief @return Список идентификаторов исходящих связей
	 * */
	QString getOutcomingLinks();

	/** @brief Добавить связь */
	void addLink( IdType const &id, /**< Идентификатор связи */
				int dir /**< Направленность связи */
				);
	/** @brief Убрать связь */
	void removeLink( IdType const &id, /**< Идентификатор связи */
					int dir /**< Направленность связи */
					);

	void clearChildren()
	{
		nodeChildren.clear();
		edgeChildren.clear();
	}

	/** @brief Отладочный вывод */
	void print(){
		qDebug() << "incoming :" << incomingLinks;
		qDebug() << "outcoming:" << outcomingLinks;
		qDebug() << "props:" <<  props;
	}
private:
	/** @brief Список дочерних элементов */
	QMap<IdType, NodeOnDiagram> nodeChildren;
	/** @brief Список дочерних линков */
	QMap<IdType, EdgeOnDiagram> edgeChildren;
	/** @brief Список входящих связей */
	QList<IdType> incomingLinks;
	/** @brief Список исходящий связей */
	QList<IdType> outcomingLinks;
};

/* class for all edge entities
 */
/** @class Link
 * 	@brief Класс для хранения информации о связях
 * 	*/
class Link: public RepoElement
{
public:
	Link( IdType const &id, TypeIdType const &type );

	/** @brief Сериализовать связь в строку
	 * 	@brief @return Строковое представление связи
	 * 	*/
	virtual QString toString();

	/** @brief Получить список объектов, к которым присоединена связь
	 * 	@brief @return Список объектов, к которым присоединена связь
	 * 	*/
	QString getObjects();

	/** @brief Присоединить объект к началу связи */
	void addObjectTo( IdType const &id /**< Идентификатор объекта */);
	/** @brief Присоединить объект к концу связи */
	void addObjectFrom( IdType const &id /**< Идентификатор объекта */);

	/** @brief Отсоединить объект от начала связи */
	void removeObjectTo( IdType const &id /**< Идентификатор объекта */);
	/** @brief Отсоединить объект от конца связи */
	void removeObjectFrom( IdType const &id /**< Идентификатор объекта */);

	/** @brief Получить идентификатор объекта, присоединенного к началу связи
	 * 	@brief @return Идентификатор объекта, присоединенного к началу связи
	 * 	*/
	IdType getFrom();
	/** @brief Получить идентификатор объекта, присоединенного к концу связи
	 * 	@brief @return Идентификатор объекта, присоединенного к концу связи
	 * 	*/
	IdType getTo();

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

	/** @brief Объекты, присоединенные к концу связи */
	QList<IdType> objectsTo;
	/** @brief Объекты, присоединенные к началу связи */
	QList<IdType> objectsFrom;
};

/* types description
 */
/** @class TypeInfo
 * 	@brief Описание типа
 * */
class TypeInfo{
public:
	/** @brief Идентификатор */
	IdType id;
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

/** @class ElementOnDiagram
 * 	@brief Описание абстрактного элемента на диаграмме
 * */
class ElementOnDiagram {
public:
	/** @brief Конструктор */
	ElementOnDiagram() : id(0), configuration("") {}
	ElementOnDiagram( IdType const &mId /**< Иднетификатор */ ) : id(mId), configuration("") {}
	/** @brief Конструктор */
	ElementOnDiagram( IdType const &mId, /**< Иднетификатор */
						QString conf /**< Конфигурация */
						) : id(mId), configuration(conf) {}

	/** @brief Изменить конфигурацию элемента */
	void setConfiguration( QString str /**< Конфигурация */)
	{
		this->configuration = str;
	}

	/** @brief Получить конфигурацию объекта
	 *	@brief @return Конфигурация объекта
	 * */
	QString getConfiguration()
	{
		return configuration;
	}

	/**	@brief Получить идентификатор элемента
	 * 	@brief @return Идентификатор элемента
	 * */
	IdType getId()
	{
		return id;
	}

private:
	/** @brief Идентификатор */
	IdType id;
	/** @brief Конфигурация */
	QString configuration;
};

/** @class NodeOnDiagram
 * 	@brief Объект на диаграмме */
class NodeOnDiagram : public ElementOnDiagram {
public:
	/** @brief Конструктор */
	NodeOnDiagram() : ElementOnDiagram(), point(QPoint(0,0)){}
	/** @brief Конструктор */
	NodeOnDiagram( IdType const &id /**< Идентификатор */) : ElementOnDiagram(id), point(QPoint(0,0))
	{
		setConfiguration(DEFAULT_NODE_CONFIGURATION);
	}
	/** @brief Конструктор */
	NodeOnDiagram( IdType const &id, /**< Идентификатор */
					QString conf, /**< Конфигурация */
					QPoint p /**< Координаты */
					) : ElementOnDiagram(id, conf), point(p) {}

	/** @brief Изменить координаты объекта */
	void setCoord( QPoint p /**< Координаты */)
	{
		this->point = p;
	}

	/** @brief Получить координаты объекта
	 * 	@brief @return Координаты объекта
	 * 	*/
	QPoint getCoord()
	{
		return point;
	}

private:
	/** @brief Координаты */
	QPoint point;
};

	/** @brief Связь на диаграмме */
class EdgeOnDiagram : public ElementOnDiagram {
public:
	/** @brief Конструктор */
	EdgeOnDiagram() : ElementOnDiagram(), position("") {}
	/** @brief Конструктор */
	EdgeOnDiagram( IdType const &id /**< Идентификатор */ ) : ElementOnDiagram(id), position("")
	{
		setConfiguration(DEFAULT_EDGE_CONFIGURATION);
	}
	/** @brief Конструктор */
	EdgeOnDiagram( IdType const &id, /**< Идентификатор */
					QString conf, /**< Конфигурация */
					QString pos /**< Расположение */
					) : ElementOnDiagram(id, conf), position(pos) {}

	/** @brief Получить расположение связи
	 *	@brief @return Расположение связи
	 * */
	QString getPosition()
	{
		return position;
	}

	/** @brief Изменить расположение связи */
	void setPosition( QString pos /**< Расположение связи */)
	{
		this->position = pos;
	}
private:
	/** @brief Расположение связи */
	QString position;
};
