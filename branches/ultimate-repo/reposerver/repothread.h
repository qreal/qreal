/** @file repothread.h
 * 	@brief Поток, обслуживающий клиента репозитория
 * */
#pragma once
#include <QThread>
#include <QTcpSocket>

#include "../common/defs.h"
#include "../common/classes.h"
#include "generated/repotypesinfo.h"

namespace reposerver
{
  
    /** @brief Массив строк */ 
    typedef QVector<QString> QStringVector;
    typedef std::pair<int, QString> IntQStringPair;
  
    /** @class QRealRepoServerThread
     * 	@brief Поток, обслуживающий клиента репозитория
     * */
    class QRealRepoServerThread: public QThread
    {
        Q_OBJECT
        
        public:
        /** @brief Конструктор */
        QRealRepoServerThread(int const &mSocketDescriptor, /**< Дескриптор сокета */
                                QObject *const parent, /**< Родительский элемент*/
                                Root *const mRoot, /**< Объект, хранящий все элементы репозитория*/
                                RepoTypesInfo *const info, /**< Информация о типах */
                                int const &id /**< Начальное значение идентификаторов элементов */
                                );
        /** @brief Основной метод потока */
        void run();
    
        public slots:
    
        signals:
            /** @brief Произошла ошибка сетевого соединения*/
            void error(QTcpSocket::SocketError socketError);
    
        private:
        /** @brief Обработать команду 
        *   @brief return Результат выполнения операции
        */
        IntQStringPair handleCommand(QString const &cmd);
    
        /** @brief Обработать команду "Получить Имя"
        *   @brief return Результат выполнения операции
        */
        IntQStringPair handleGetName(QStringVector const &params);
        /** @brief Обработать команду "Установить имя"
        *   @brief return Результат выполнения операции
        */
        IntQStringPair handleSetName(QStringVector const &params);
        /** @brief Обработать команду "Установить идентификатор родителя"
        *   @brief return Результат выполнения операции
        */
        IntQStringPair handleSetParent(QStringVector const &params);
        /** @brief Обработать команду "Получить идентификатор родителя"
        *   @brief return Результат выполнения операции
        */
        IntQStringPair handleGetParent(QStringVector const &params);
        /** @brief Обработать команду "Создать сущность"
        *   @brief return Результат выполнения операции
        */
        IntQStringPair handleCreateEntity(QStringVector const &params);    
        /** @brief Обработать команду "Поместить сущность на другую диаграмму"
        *   @brief return Результат выполнения операции
        */
        IntQStringPair handleCopyEntity(QStringVector const &params);    
        /** @brief Обработать команду "Удалить сущность"
        *   @brief return Результат выполнения операции
        */
        IntQStringPair handleDeleteEntity(QStringVector const &params);
        /** @brief Обработать команду "Получить число типов"
        *   @brief return Результат выполнения операции
        */
        IntQStringPair handleGetTypesCount(QStringVector const &params);
        /** @brief Обработать команду "Получить все типы"
        *   @brief return Результат выполнения операции
        */
        IntQStringPair handleGetAllTypes(QStringVector const &params);
        /** @brief Обработать команду "Получить информацию о типе"
        *   @brief return Результат выполнения операции
        */
        IntQStringPair handleGetTypeInfo(QStringVector const &params);
        /** @brief Обработать команду "Получить информацию о типе по его имени"
        *   @brief return Результат выполнения операции
        */
        IntQStringPair handleGetTypeByName(QStringVector const &params);
        /** @brief Обработать команду "Получить типы по метатипу"
        *   @brief return Результат выполнения операции
        */
        IntQStringPair handleGetTypesByMetatype(QStringVector const &params);
        /** @brief Обработать команду "Получить список идентификаторов объектов данного типа"
        *   @brief return Результат выполнения операции
        */
        IntQStringPair handleGetObjectsByType(QStringVector const &params);
        /** @brief Обработать команду "Получить данные об объекте"
        *   @brief return Результат выполнения операции
        */
        IntQStringPair handleGetObjectData(QStringVector const &params);
        /** @brief Обработать команду "Получить список идентификаторов дочерных элементов"
        *   @brief return Результат выполнения операции
        */
        IntQStringPair handleGetChildren(QStringVector const &params);
        /** @brief Обработать команду "Получить описание сущности"
        *   @brief return Результат выполнения операции
        */
        IntQStringPair handleGetDescription(QStringVector const &params);
        /** @brief Обработать команду "Установить описание сущности"
        *   @brief return Результат выполнения операции
        */
        IntQStringPair handleSetDescription(QStringVector const &params);
        /** @brief Обработать команду "Установить координаты сущности"
        *   @brief return Результат выполнения операции
        */
        IntQStringPair handleGetPosition(QStringVector const &params);
        /** @brief Обработать команду "Получить координаты сущности"
        *   @brief return Результат выполнения операции
        */
        IntQStringPair handleSetPosition(QStringVector const &params);
        /** @brief Обработать команду "Получить конфигурацию сущности"
        *   @brief return Результат выполнения операции
        */
        IntQStringPair handleGetConfiguration(QStringVector const &params);
        /** @brief Обработать команду "Установить конфигурацию сущности"
        *   @brief return Результат выполнения операции
        */
        IntQStringPair handleSetConfiguration(QStringVector const &params);
        /** @brief Обработать команду "Установить значение свойства"
        *   @brief return Результат выполнения операции
        */
        IntQStringPair handleSetProperty(QStringVector const &params);
        /** @brief Обработать команду "Получить значение свойства"
        *   @brief return Результат выполнения операции
        */
        IntQStringPair handleGetProperty(QStringVector const &params);
        /** @brief Обработать команду "Добавить связь"
        *   @brief return Результат выполнения операции
        */
        IntQStringPair handleAddLink(QStringVector const &params);
        /** @brief Обработать команду "Удалить связь"
        *   @brief return Результат выполнения операции
        */
        IntQStringPair handleRemoveLink(QStringVector const &params);
        /** @brief Обработать команду "Получить объект"
        *   @brief return Результат выполнения операции
        */
        IntQStringPair handleGetEntireObject(QStringVector const &params);
        /** @brief Обработать команду "Получить связи, ассоциированные с объектом"
        *   @brief return Результат выполнения операции
        */
        IntQStringPair handleGetLinksByObject(QStringVector const &params);
        /** @brief Обработать команду "Получить объекты, ассоциированные со связью"
        *   @brief return Результат выполнения операции
        */
        IntQStringPair handleGetObjectsByLink(QStringVector const &params);
    
        /** @brief Отправить ответ с кодом ошибки операции */
        IntQStringPair ReportError(const int &errorCode);
        /** @brief Отправить ответ об успешном завершении операции */
        IntQStringPair ReportSuccess(const QString &data);
		/** @brief Восстановить состояние репозитория */
        bool IsParamsNumberCorrect(QStringVector const &params, 
                                    QString const &command, 
                                    int const &paramsNum
                                    ) const;
		/** @brief Восстановить состояние репозитория */
        void TryToRestoreState();
    
		/** @brief Дескриптор сокета */
        int mSocketDescriptor;
		/** @brief Объект, хранящий данные о всех сущностях */
        Root *mRoot;
		/** @brief Объект, хранящий данные о всех типах элементов */
        RepoTypesInfo *mTypesInfo;
		/** @brief Вспомогательный счетчик */
        int mCounter;
    
		/** @brief Строка для формирования записи в лог */
        mutable QString mLog;
    };
  
}
