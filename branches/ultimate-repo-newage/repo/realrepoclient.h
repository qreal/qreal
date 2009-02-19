/** @file realrepoclient.h
 * 	@brief API для доступа к репозиторию
 * */
#pragma once

#include <QTcpSocket>
#include "../common/defs.h"
#include "../common/classes.h"
#include "../common/realrepoapiclasses.h"

/** @class RealRepoClient
 * 	@brief Клиент репозитория
 * */
class RealRepoClient : public QObject
{
	Q_OBJECT;

public:
	RealRepoClient( const QString& addr, const int port, QObject *parent = 0 );
	~RealRepoClient();

	/** @brief Получить имя элемента
	 *	@brief @return Имя элемента
	 * */
	QString getName( int id /**< Идентификатор элемента */);
	/** @brief Установить имя элемента
	 *	@brief @return Код возврата
	 * */
	int setName( int id, /**< Идентификатор элемента */
				QString name /**< Имя элемента*/
				);

	/** @brief Установить позицию элемента */
	void setPosition( int id, /**< Идентификатор элемента */
						int parent, /**< Идентификатор родительского элемента */
						int x, /**< Координата X */
						int y /**< Координата Y */
						);

	/** @brief Установить описание элемента */
	void setDescription( int id, /**< Идентификатор элемента */
						QString desc /**< Описание элемента */
						);
	/** @brief Получить описание элемента
	 *	@brief @return Описание элемента
	 * */
	QString getDescription( int id /**< Идентификатор элемента */);

	/** @brief Установить конфигурацию элемента
	 *	@brief @return Код возврата
	 * */
	int setConfiguration( int id, /**< Идентификатор элемента */
						int parent, /**< Идентификатор диаграммы */
						QString conf /**< Конфигурация */
						);
	/** @brief Получить конфигурацию элемента
	 *	@brief @return Конфигурация элемента
	 * */
	QString getConfiguration( int id, /**< Идентификатор элемента */
							int parent /**< Идентификатор диаграммы */
							);

	/** @brief Сохранить значение свойства
	 *	@brief @return Код возврата
	 * */
	int setPropValue( int id, /**< Идентификатор элемента */
					QString name, /**< Название свойства */
					QString value /**< Значение свойства */
					);
	/** @brief Получить значение свойства
	 *	@brief @return Значение свойства
	 * */
	QString getPropValue(  int id, /**< Идентификатор элемента */
						QString name /**< Название свойства */
						);

	/** @brief Создать объект
	 *	@brief @return Идентификатор созданного объекта
	 * */
	int createObject( int type, /**< Тип объекта */
					QString name  /**< Имя объекта */
					);
	/** @brief Поместить существующий объект на другую диаграмму
	 *	@brief @return Идентификатор созданного объекта
	 * */
	int copyEntity( int type, /**< Тип объекта */
					int id,  /**< Идентификатор объекта */
					int newParent, /**< Идентификатор диаграммы, на которую помещаем объект */
					int oldParent,
					bool full = false
					);
	/** @brief Создать элемент с указанным родителем
	 *	@brief @return Идентификатор созданного объекта
	 * */
	int createObjectWithParent(int type, /**< Тип объекта*/
							QString name, /**< Имя */
							int parent /**< Идентификатор родительского объекта */
							);
	/** @brief Создать связь
	 *	@brief @return Идентификатор созданной связи
	 * */
	int createLink(QString name /**< Имя */);
	/** @brief Создать связь заданного типа
	 *	@brief @return Идентификатор созданной связи
	 * */
	int createLinkWithType(QString name, /**< Имя */
							QString type /**< Тип */
							);
	/** @brief Создать связь заданного типа и положить в заданный контейнер
	 *	@brief @return Идентификатор созданной связи
	 * */
	int createLinkWithParent(int type, /**< Тип */
								QString name, /**< Имя */
								int parent
							);

	/** @brief Удалить объект */
	void deleteObject( int id, /**< Идентификатор */
					int parent /**< Идентификатор родительского объекта */
					);
	/** @brief Удалить объект вообще */
	void deleteObject( int id /**< Идентификатор */ );
	/** @brief Удалить связь */
	void deleteLink( int id, /**< Идентификатор */
					int parent /**< Идентификатор родительского объекта */
					);
	/** @brief Удалить связь вообще */
	void deleteLink( int id /**< Идентификатор */ );

	/** @brief Присоединить связь к объекту */
	void addLink( int obj_id, /**< Идентификатор объекта */
				int link_id, /**< Идентификатор связи */
				int direction /**< Направленность связи */
				);
	/** @brief Отсоединить связь от объекта */
	void removeLink( int obj_id, /**< Идентификатор объекта */
					int link_id, /**< Идентификатор связи */
					int direction /**< Направленность связи*/
					);

	/** @brief Получить число типов
	 *	@brief @return Число типов
	 * */
	int getTypesCount();
	/** @brief Полчить список идентификаторов типов
	 *	@brief @return Список идентификаторов типов
	 * */
	qRealTypes::QIntList getAllTypes();
	/** @brief Получить описание типа
	 *	@brief @return Описание типа
	 * */
	TypeInfo getTypeInfo( int id /**< Идентификтор типа*/);

	/** @brief Получить список объектов заданного типа
	 *	@brief @return Список объектов заданного типа
	 * */
	QString getObjectsByType( int type /**< Идентификатор типа */);
	/** @brief Получить список объектов заданного типа
	 *	@brief @return Список объектов заданного типа
	 * */
	qRealTypes::QIntList getObjectsListByType( int type /**< Идентификатор типа */);
	/** @brief Получить список связей
	 *	@brief @return Список связей
	 * */
	qRealTypes::QIntList getLinks();

	/** @brief Получить данные объекта
	 *	@brief @return Данные объекта
	 * */
	QString getObjectData( int id /**< Идентификатор объекта */);
	/** @brief Получить объект
	 *	@brief @return Объект
	 * */
	QString getEntireObject( int id /**< Идентификатор объекта */);
	/** @brief Получить объект
	 *	@brief @return Объект
	 * */
	qRealTypes::RealObject getObjectById( int id /**< Идентификатор объекта */);
	/** @brief Получить связь
	 *	@brief @return Связь
	 * */
	qRealTypes::RealLink getLinkById( int id /**< Идентификатор объекта */);

	/** @brief Получить список связей, ассоциированных с данным объектом
	 *	@brief @return
	 * */
	QString getLinksByObject( int id, /**< Идентификатор объекта */
							int direction /**< Направленность связи */
							);
	/** @brief Получить список объектов, ассоциированных с данной связью
	 *	@brief @return Список объектов, ассоциированных с данной связью
	 * */
	QString getObjectsByLink( int id /**< Идентификатор объекта */);

	/** @brief Получить список дочерних элементов
	 *	@brief @return Список дочерних элементов
	 * */
	QString getChildren( int id /**< Идентификатор объекта */);

	/** @brief Получить список элементов, в которых содержится данный
	 *	@brief @return Список элементов - родителей
	 * */
	qRealTypes::QIntList getParents( int id /**< Идентификатор объекта */);

	/** @brief Получить позицию элемента
	 *	@brief @return Позиция элемента
	 * */
	QString getPosition(int id /**< Идентификатор объекта */,
						int parent /**< Идентификатор диаграммы */
						);
	/** @brief Получить код последней ошибки
	 *	@brief @return код последней ошибки
	 * */
	int getLastError();

	/** @brief Получить состояние сокета
	 *	@brief @return Состояние сокета
	 * */
	int state()
	{
		return socket->state();
	}

	/** @brief Получить статус сокета
	 *	@brief @return Статус сокета
	 * */
	int isValid()
	{
		return socket->isValid();
	}

	/** @brief Получить список типов данного метатипа
	 *	@brief @return Список типов данного метатипа
	 * */
	qRealTypes::QIntList getTypesByMetaType( const qRealTypes::MetaType /**< Метатип */);

	/** @brief Получить тип по его идентификатору
	 *	@brief @return Тип элементов
	 * */
	qRealTypes::RealType getTypeById( const int id /**< Идентификатор типа*/);
	/** @brief Получить тип по его названию
	 *	@brief @return Тип элементов
	 * */
	qRealTypes::RealType getTypeByName( const QString name /**< Название типа */);
	/** @brief Получить идентификатор типа по его имени
	 *	@brief @return Идентификатор типа
	 * */
	int getTypeIdByName( const QString name /**< Название типа */);

public slots:
	/** @brief Выдать описание ошибки */
	void displayError( QAbstractSocket::SocketError err/**< Ошибка */);

private:
	/** @brief Послать запрос серверу репозитория
	 *	@brief @return Ответ сервера
	 * */
	QString sendData( QString data );

	/** @brief Обработать код ошибки */
	QString extractData(QString const &resp /**< Ответ сервера репозитория */);
	/** @brief Сокет */
	QTcpSocket *socket;
	/** @brief Код последней ошибки */
	int m_error;
};
