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

// TODO: Убрать отсюда все низкоуровневые методы, возвращать уже нормальные списки!!!
class RealRepoClient : public QObject
{
	Q_OBJECT;

public:
	RealRepoClient(const QString& addr, const int port, QObject *parent = 0);
	~RealRepoClient();

	/** @brief Получить имя элемента
	 *	@brief @return Имя элемента
	 * */
	QString getName(IdType const &id/**< Идентификатор элемента */);
	/** @brief Установить имя элемента
	 *	@brief @return Код возврата
	 * */
	int setName(IdType const &id, /**< Идентификатор элемента */
				QString name /**< Имя элемента*/
				);

	/** @brief Установить позицию элемента */
	void setPosition(IdType const &id, /**< Идентификатор элемента */
						IdType const &parent, /**< Идентификатор родительского элемента */
						int x, /**< Координата X */
						int y /**< Координата Y */
						);

	/** @brief Установить описание элемента */
	void setDescription(IdType const &id, /**< Идентификатор элемента */
						QString desc /**< Описание элемента */
						);
	/** @brief Получить описание элемента
	 *	@brief @return Описание элемента
	 * */
	QString getDescription(IdType const &id/**< Идентификатор элемента */);

	/** @brief Установить конфигурацию элемента
	 *	@brief @return Код возврата
	 * */
	int setConfiguration(IdType const &id, /**< Идентификатор элемента */
						IdType const &parent, /**< Идентификатор диаграммы */
						QString conf /**< Конфигурация */
						);
	/** @brief Получить конфигурацию элемента
	 *	@brief @return Конфигурация элемента
	 * */
	QString getConfiguration(IdType const &id, /**< Идентификатор элемента */
							IdType const &parent /**< Идентификатор диаграммы */
							);

	/** @brief Сохранить значение свойства
	 *	@brief @return Код возврата
	 * */
	int setPropValue(IdType const &id, /**< Идентификатор элемента */
					QString name, /**< Название свойства */
					QString value /**< Значение свойства */
					);
	/** @brief Получить значение свойства
	 *	@brief @return Значение свойства
	 * */
	QString getPropValue(IdType const &id, /**< Идентификатор элемента */
						QString name /**< Название свойства */
						);

	/** @brief Создать объект
	 *	@brief @return Идентификатор созданного объекта
	 * */
	IdType createObject(TypeIdType const &type, /**< Тип объекта */
					QString name  /**< Имя объекта */
					);
	/** @brief Поместить существующий объект на другую диаграмму
	 *	@brief @return Идентификатор созданного объекта
	 * */
	IdType copyEntity(TypeIdType const &type, /**< Тип объекта */
					IdType const &id,  /**< Идентификатор объекта */
					IdType const &newParent, /**< Идентификатор диаграммы, на которую помещаем объект */
					IdType const &oldParent,
					bool full = false
					);
	IdType reparentEntity( IdType const &id, 
					IdType const &newParent, 
					IdType const &oldParent );
	/** @brief Создать элемент с указанным родителем
	 *	@brief @return Идентификатор созданного объекта
	 * */
	IdType createObjectWithParent(TypeIdType const &type, /**< Тип объекта*/
							QString name, /**< Имя */
							IdType const &parent /**< Идентификатор родительского объекта */
							);
	/** @brief Создать связь
	 *	@brief @return Идентификатор созданной связи
	 * */
	IdType createLink(QString name /**< Имя */);
	/** @brief Создать связь заданного типа
	 *	@brief @return Идентификатор созданной связи
	 * */
	IdType createLinkWithType(QString name, /**< Имя */
							TypeIdType type /**< Тип */
							);
	/** @brief Создать связь заданного типа и положить в заданный контейнер
	 *	@brief @return Идентификатор созданной связи
	 * */
	IdType createLinkWithParent(TypeIdType const &type, /**< Тип */
								QString name, /**< Имя */
								IdType const &parent
							);

	/** @brief Удалить объект */
	void deleteObject(IdType const &id, /**< Идентификатор */
					IdType const &parent /**< Идентификатор родительского объекта */
					);
	/** @brief Удалить объект вообще */
	void deleteObject(IdType const &id/**< Идентификатор */ );
	/** @brief Удалить связь */
	void deleteLink(IdType const &id, /**< Идентификатор */
					IdType const &parent /**< Идентификатор родительского объекта */
					);
	/** @brief Удалить связь вообще */
	void deleteLink(IdType const &id/**< Идентификатор */ );

	/** @brief Присоединить связь к объекту */
	void addLink(IdType const &obj_id, /**< Идентификатор объекта */
				IdType const &link_id, /**< Идентификатор связи */
				int direction /**< Направленность связи */
				);
	/** @brief Отсоединить связь от объекта */
	void removeLink(IdType const &obj_id, /**< Идентификатор объекта */
					IdType const &link_id, /**< Идентификатор связи */
					int direction /**< Направленность связи*/
					);

	/** @brief Получить число типов
	 *	@brief @return Число типов
	 * */
	int getTypesCount();
	/** @brief Полчить список идентификаторов типов
	 *	@brief @return Список идентификаторов типов
	 * */
	qRealTypes::TypeIdTypeList getAllTypes();

#if 0 // UNUSED & CONFUSING
	/** @brief Получить описание типа
	 *	@brief @return Описание типа
	 * */
	TypeInfo getTypeInfo(IdType const &id/**< Идентификтор типа*/);
#endif

	/** @brief Получить список объектов заданного типа
	 *	@brief @return Список объектов заданного типа
	 * */
	QString getObjectsByType(TypeIdType const &type /**< Идентификатор типа */);
	/** @brief Получить список объектов заданного типа
	 *	@brief @return Список объектов заданного типа
	 * */
	qRealTypes::IdTypeList getObjectsListByType(TypeIdType const &type /**< Идентификатор типа */);
	/** @brief Получить список связей
	 *	@brief @return Список связей
	 * */
	qRealTypes::IdTypeList getLinks();

	/** @brief Получить данные объекта
	 *	@brief @return Данные объекта
	 * */
	QString getObjectData(IdType const &id/**< Идентификатор объекта */);
	/** @brief Получить объект
	 *	@brief @return Объект
	 * */
	QString getEntireObject(IdType const &id/**< Идентификатор объекта */);
	/** @brief Получить объект
	 *	@brief @return Объект
	 * */
	qRealTypes::RealObject getObjectById(IdType const &id/**< Идентификатор объекта */);
	/** @brief Получить связь
	 *	@brief @return Связь
	 * */
	qRealTypes::RealLink getLinkById(IdType const &id/**< Идентификатор объекта */);

	/** @brief Получить список связей, ассоциированных с данным объектом
	 *	@brief @return
	 * */
	QString getLinksByObject(IdType const &id, /**< Идентификатор объекта */
							int direction /**< Направленность связи */
							);
	/** @brief Получить список объектов, ассоциированных с данной связью
	 *	@brief @return Список объектов, ассоциированных с данной связью
	 * */
	QString getObjectsByLink(IdType const &id/**< Идентификатор объекта */);

	/** @brief Получить список дочерних элементов
	 *	@brief @return Список дочерних элементов
	 * */
	QString getChildren(IdType const &id/**< Идентификатор объекта */);

	/** @brief Получить список элементов, в которых содержится данный
	 *	@brief @return Список элементов - родителей
	 * */
	qRealTypes::IdTypeList getParents(IdType const &id/**< Идентификатор объекта */);

	/** @brief Получить позицию элемента
	 *	@brief @return Позиция элемента
	 * */
	QString getPosition(IdType const &id/**< Идентификатор объекта */,
						IdType const &parent /**< Идентификатор диаграммы */
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
	qRealTypes::TypeIdTypeList getTypesByMetaType(const qRealTypes::MetaType metatype/**< Метатип */);

	/** @brief Получить тип по его идентификатору
	 *	@brief @return Тип элементов
	 * */
	qRealTypes::RealType getTypeById(TypeIdType const &id/**< Идентификатор типа*/);
	/** @brief Получить тип по его названию
	 *	@brief @return Тип элементов
	 * */
	qRealTypes::RealType getTypeByName(QString const &name /**< Название типа */);
	/** @brief Получить идентификатор типа по его имени
	 *	@brief @return Идентификатор типа
	 * */
	IdType getTypeIdByName(QString const &name /**< Название типа */);

	/** @brief Получить все объекты, что есть в репозитории.
	 *	@brief @return Список объектов в репозитории.
	 * */
	qRealTypes::IdTypeList getAllObjects();

	/** @brief Устроить в репозитории экстерминатус. Разумеется, в целях отладки. */
	void clearRepository();

public slots:
	/** @brief Выдать описание ошибки */
	void displayError( QAbstractSocket::SocketError err/**< Ошибка */);

private:
	/** @brief Послать запрос серверу репозитория
	 *	@brief @return Ответ сервера
	 * */
	QString sendData(QString const &data);

	/** @brief Обработать код ошибки */
	QString extractData(QString const &resp /**< Ответ сервера репозитория */);
	/** @brief Сокет */
	QTcpSocket *socket;
	/** @brief Код последней ошибки */
	int m_error;
};
