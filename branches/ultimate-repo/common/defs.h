/**	@file defs.h
 * 	@brief Объявления используемых в проекте констант
 * */
#pragma once

/** @brief Операция завершилась успешно*/
const int ERR_STATUS_OK	        = 1;
/** @brief Некорректные параметры */
const int ERR_INCORRECT_PARAMS  = 2;
/** @brief Некорректный запрос */
const int ERR_INCORRECT_REQUEST = 4;

/** @brief Самая страшная ошибка вообще*/
const int ERR_UNKNOWN_ERROR	= 888;

/** @brief Команда "Создать сущность" */
const int CMD_CREATE_ENTITY		= 1;
/** @brief Код команды "Удалить сущность" */
const int CMD_DELETE_ENTITY		= 2;
/** @brief Код команды "Установить имя сущности" */
const int CMD_SET_NAME			= 3;
/** @brief Код команды "Получить имя сущности" */
const int CMD_GET_NAME			= 4;
/** @brief Код команды "Установить расположение сущности" */
const int CMD_SET_POSITION		= 5;
/** @brief Код команды "Получить расположение сущности" */
const int CMD_GET_POSITION		= 6;
/** @brief Код команды "Установить конфигурацию сущности" */
const int CMD_SET_CONFIGURATION	= 7;
/** @brief Код команды "Получить конфигурацию сущности" */
const int CMD_GET_CONFIGURATION	= 8;
/** @brief Код команды "Устновить значение свойства" */
const int CMD_SET_PROPERTY		= 9;
/** @brief Код команды "Получить значение свойства" */
const int CMD_GET_PROPERTY		= 10;
/** @brief Код команды "Установить описание сущности" */
const int CMD_SET_DESCRIPTION	= 11;
/** @brief Код команды "Получить описание сущности" */
const int CMD_GET_DESCRIPTION	= 12;
/** @brief Код команды "Установить идентификатор родителя" */
const int CMD_SET_PARENT		= 13;
/** @brief Код команды "Получить идентификатор родителя" */
const int CMD_GET_PARENT		= 14;
/** @brief Код команды "Добавить существущий объект на другую диаграмму" */
const int CMD_COPY_ENTITY		= 15;
const int CMD_FULLCOPY_ENTITY	= 16;
const int CMD_REPARENT_ENTITY	= 17;

/** @brief Код команды "Получить сущность" */
const int CMD_GET_ENTITY		= 20;
/** @brief Код команды "Получить объекты данного типа" */
const int CMD_GET_OBJECTS_BY_TYPE	= 21;
/** @brief Код команды "Получить данные объекта" */
const int CMD_GET_OBJECT_DATA		= 22;
/** @brief Код команды "Получить объект" */
const int CMD_GET_ENTIRE_OBJECT		= 23;
/** @brief Код команды "Получить список детей объекта" */
const int CMD_GET_CHILDREN		= 24;
/** @brief Код команды "Получить связи объекта" */
const int CMD_GET_LINKS_BY_OBJECT	= 25;
/** @brief Код команды "Получить объекты, к которым присоединена связь" */
const int CMD_GET_OBJECTS_BY_LINK	= 26;
/** @brief Код команды "Получить список контейнеров, содержащих объект" */
const int CMD_GET_CONTAINERS		= 27;

/** @brief Код команды "Получить число типов" */
const int CMD_GET_TYPES_COUNT		= 31;
/** @brief Код команды "Получить все типы" */
const int CMD_GET_ALL_TYPES		= 32;
/** @brief Код команды "Получить типы данного метатипа" */
const int CMD_GET_TYPES_BY_METATYPE	= 33;
/** @brief Код команды "Получить описание типа" */
const int CMD_GET_TYPE_INFO		= 34;
/** @brief Код команды "Получить тип по названию" */
const int CMD_GET_TYPE_BY_NAME		= 35;

/** @brief Код команды "Добавить связь к объекту" */
const int CMD_ADD_LINK			= 51;
/** @brief Код команды "Убрать связь объекта" */
const int CMD_REMOVE_LINK		= 52;

// Команды, которые по идее нужны только для отладки.
/** @brief Код команды "Получить все объекты" */
const int CMD_GET_ALL_OBJECTS	= 101;
/** @brief Код команды "Удалить всё из репозитория" */
const int CMD_CLEAR_ALL			= 102;

/** @brief Cущности не существует */
const int TYPE_INVALID  = 0;
/** @brief Тип сущности "Объект" */
const int TYPE_OBJECT	= 1;
/** @brief Тип сущности "Связь" */
const int TYPE_LINK	= 2;

/** @brief Некорректный идентификатор */
const QString INVALID_ID 	= "-1";

/** @brief Тип связи "Входящая"*/
const int INCOMING_LINK		= 1;
/** @brief Тип связи "Исходящая"*/
const int OUTCOMING_LINK	= 2;

/** @brief Конфигурация элемента по умолчанию*/
const QString DEFAULT_NODE_CONFIGURATION = "(0,0);(50,0);(50,70);(0,70)";
/** @brief Конфигурация связи по умолчанию*/
const QString DEFAULT_EDGE_CONFIGURATION = "(0,0);(200,60)";

