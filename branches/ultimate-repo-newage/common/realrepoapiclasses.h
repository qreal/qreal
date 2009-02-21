/** @file realrepoapiclasses.h
 * 	@class Классы, используемые в API репозитория
 * */
#pragma once

#include <QList>
#include <QMap>
#include <QString>
#include <QDebug>
#include "defs.h"

#include "defs.h"

class RealRepoClient;

/** @brief Пространство имен классов API репозитория */
namespace qRealTypes
{
	/** Список целых чисел */
	typedef QList<int> QIntList;

	// common stuff for all classes
	/** @class RealNamedEntity
	 * 	@brief Именованая сущность. Базовый класс для классов объектов и связей
	 * 	*/
	class RealNamedEntity
	{
	public:
		RealNamedEntity(): m_id(-1),  m_type(-1), m_name("noname"),
			m_description("noname"), client(NULL)
		{
			m_properties.clear();
		}

		/** @brief Получить идентификатор сущности
		 *	@brief @return Ищентификатор сущности
		 * */
		int getId();
		/** @brief Установить идентификатор сущности */
		void setId( const int id /**< Идентификатор */); // to be removed soon

		/** @brief Получить имя сущности
		 *	@brief @return Имя сущности
		 * */
		QString getName();
		/** @brief Устновить имя сущности */
		void setName( const QString& name /**< Имя */);

		/** @brief Получить идентификатор типа сущности
		 *	@brief @return Идентификатор типа сущности
		 * */
		int getTypeId() const;
		/** @brief Устновить идентификатор типа сущности */
		void setTypeId( const int id /**< Идентификатор типа */);

		/** @brief Получить описание сущности
		 *	@brief @return Описание сущности
		 * */
		QString getDescription();
		/** @brief Устновить описание сущности */
		void setDescription( const QString& desc /**< Описание */);

		/** @brief Установить значение свойства */
		void setProperty( const QString& name, /**< Название свойства */
						const QString& val /**< Значение свойства */
						);
		/** @brief Получить значение свойства
		 *	@brief @return Значение свойства
		 * */
		QString getProperty( const QString& name /**< Название свойства */); // returns "" in case of empty property value
		/** @brief Получить число свойств элемента
		 *	@brief @return Число свойств элемента
		 * */
		int getPropertiesCount() const;

		/** @brief Установить указатель на клиент репозитория */
		void setRepoClient( RealRepoClient * repo /**< Указатель на клиент репозитория */);

	protected:
		/** @brief Идентификатор */
		int m_id;
		/** @brief Тип */
		int m_type;
		/** @brief Имя */
		QString m_name;
		/** @brief Описание */
		QString m_description;
		/** @brief Свойства и их значения */
		QMap<QString, QString> m_properties;
		/** @brief Клиент репозитория */
		RealRepoClient  *client;
	};

	// Metatype description
	/** @brief Метатип */
	enum MetaType
	{
		object, /**< Тип объектов диаграмм (например, invoke, exit, if, foreach, throw, reply)*/
		link, /**< Связи */
		dataType, /**< Все основные типы данных: основные (int, double, string, char) и определяемые пользователем (структуры, записи, xsd и т.п.)*/
		rawType, /**< Строковые типы данных (wsdl, xsd, разного рода конфигурации и т.п.)*/
	};

	/** @brief Метатип */
	typedef enum MetaType MetaType;

	// Type description
	/** @brief Тип элементов */
	class RealType : public RealNamedEntity
	{
	public:
		RealType(): m_metatype(object) { m_objects.clear(); }
		// In debug purposes -- subject to be removed.
		~RealType() { /*qDebug() << "-destroying " << getName() << endl;*/ }

		/** @brief Получить метатип
		 *	@brief @return Метатип
		 * */
		MetaType getMetaType() const;
		/** @brief Установить метатип */
		void setMetaType( const MetaType mtype /**< Метатип */ );

		/** @brief Получить список объектов данного типа
		 *	@brief @return Список объектов данного типа
		 * */
		QIntList getObjects() const; // returns all objects of this particular type

		/** @brief Добавить объект */
		void addObject( int id /**< Идентификатор объекта */);
		/** @brief Удалить объект */
		void deleteObject( int id /**< Идентификатор объекта */);

		/** @brief Сериализовать данные о типе в строку
		 *	@brief @return Строковое представление данных о типе
		 * */
		QString toString() const;
		/** @brief Десериализовать данные о типе*/
		void loadFromString( const QString& str /**< Строковое представление данных о типе */);

	private:

		/** @brief Метатип */
		MetaType m_metatype;
		/** @brief Список объектов данного типа */
		QIntList m_objects;
	};

	// Node elements
	/** @class RealObject
	 * 	@brief Объект на диаграмме */
	class RealObject : public RealNamedEntity
	{
	public:
		RealObject(): m_visibility(true) {}
		/** @brief Узнать, является ли объект визуальным
		 *	@brief @return Является ли объект визуальным
		 * */
		bool getVisibility() const; // is it visible on the diagrams or not
		/** @brief */
		void setVisibility( const bool flag /**< Индикатор визуальности объекта */);

		/** @brief Установить конфигурацию элемента
		 *	@brief @return Код возврата
		 * */
		int setConfiguration(int parent, /**< Идентификатор диаграммы */
							QString conf /**< Конфигурация */
							);
		/** @brief Получить конфигурацию элемента
		 *	@brief @return Конфигурация элемента
		 * */
		QString getConfiguration(int parent /**< Идентификатор диаграммы */ );

		/** @brief Получить идентификаторы всех контейнеров которым принадлежит объект
		 *	@brief @return Идентификаторы контейнеров объекта
		 * */
		QIntList getContainerId();
		/** @brief Добавить объект в указанный контейнер */
		void addToContainer(const int id);
		/** @brief Удалить объект из указанного контейнера */
		void  deleteFromContainer(const int id);

		/** @brief Установить идентификатор контейнера объекта. Deprecated. */
		void setContainerId(const int id);

		/** @brief Получить список дочерних элементов объекта
		 *	@brief @return Список дочерних элементов объекта
		 * */
		QIntList getChildElements();
		/** @brief Добавить дочерний элемент*/
		void addChildElement( const int id /**< Идентификатор элемента */);
		/** @brief Удалить элемент из списка дочерних*/
		void deleteChildElement( const int id /**< Идентификатор элемента */);

		/** @brief Получить список связей, ассоциированных с данным объектом
		 *	@brief @return Список связей, ассоциированных с данным объектом
		 * */
		QIntList getAllLinks() const;
		/** @brief Получить входящие в объект связи
		 *	@brief @return Список входящих связей
		 * */
		QIntList getIncomingLinks() const;
		/** @brief Получить исходящие из объекта связи
		 *	@brief @return Список исходящих связей
		 * */
		QIntList getOutcomingLinks() const;

		/** @brief Добавить входящую связь */
		void addIncomingLink( const int id /**< Идентификатор связи */);
		/** @brief Добавить исходящую связь*/
		void addOutcomingLink( const int id /**< Идентификатор связи */);

		/** @brief Убрать входящую связь */
		void removeIncomingLink( const int id /**< Идентификатор связи */);
		/** @brief Убрать исходящую связь */
		void removeOutcomingLink( const int id /**< Идентификатор связи */);

	private:
		/** @brief Индикатор визуальности объекта */
		bool m_visibility;
		/** @brief Конфигурация */
		QString m_configuration;

		/** @brief Список входящих связей */
		QIntList m_incomingLinks;
		/** @brief Список исходящих связей */
		QIntList m_outcomingLinks;
	};

	// Edge elements
	/** @class RealLink
	 * 	@brief Связь на диаграмме
	 * */
	class RealLink : public RealNamedEntity
	{
	public:
		RealLink(): m_fromId(-1), m_toId(-1) { }
		/** @brief Получить идентификатор объекта, присоединенного к началу связи
		 *	@brief @return Идентификатор объекта, присоединенного к началу связи
		 * */
		int getFromId();  // source node
		/** @brief Установить идентификатор объекта, присоединенного к началу связи*/
		void setFromId( const int id /**< Идентификатор объекта, присоединенного к началу связи */ );

		/** @brief Получить идентификатор объекта, присоединенного к концу связи
		 *	@brief @return Идентификатор объекта, присоединенного к концу связи
		 * */
		int getToId();    // target node
		/** @brief Установить идентификатор объекта, присоединенного к концу связи */
		void setToId( const int id /**< Идентификатор объекта, присоединенного к концу связи */);

		/** @brief Отсоединить объект от начала связи */
		void removeFrom( const int id /**< Идентификатор отсоединяемого объекта */);
		/** @brief Отсоединить идентификатор от конца связи */
		void removeTo( const int id /**< Идентификатор отсоединяемого объекта */);

	private:

		/** @brief Идентификатор объекта, присоединенного к началу связи*/
		int m_fromId;
		/** @brief Идентификатор объекта, присоединенного к концу связи*/
		int m_toId;
	};
}
