/** @file realrepomodel.h
 *	@brief Основная модель данных
 * */
#pragma once

#include <QtCore/QAbstractItemModel>
#include <QtCore/QModelIndex>
#include <QtCore/QVariant>
#include <QtCore/QHash>
#include <QtCore/QPoint>

#include <QtGui/QPolygon>

#include <QUndoStack>
#include <QUndoView>

#include "realrepoinfo.h"
#include "realrepoclient.h"
#include "../common/classes.h"
#include "realrepoundocommands.h"

/** @class RealRepoModel
 * 	@brief Класс основной модели данных
 * */
class RealRepoModel : public QAbstractItemModel
{
	Q_OBJECT

	public:
		/** @brief Конструктор */
		RealRepoModel( const QString &addr, const int port, QObject *parent = 0 );
		/** @brief Деструктор */
		~RealRepoModel();

		/** @brief Получить индекс элемента
		 *	@brief @return Индек элемента
		 * */
		QModelIndex index(int row, /**< Номер строки */
						int column, /**< Номер столбца */
						const QModelIndex &parent = QModelIndex() /**< Индекс родительского элемента */
						) const;
		/** @brief Получить индекс родительского элемента
		 *	@brief @return Индекс родительского элемента
		 * */
		QModelIndex parent(const QModelIndex &child /**< Индекс элемента */) const;

		/** @brief Получить заголовок
		 *	@brief @return Заголовок
		 * */
		QVariant headerData(int section, /**< Номер секции */
						Qt::Orientation orientation, /**< Ориентация */
						int role = Qt::DisplayRole /**< Роль */
						) const;

		/** @brief Получить данные элемента
		 *	@brief @return Данные элемента
		 * */
		QVariant data(const QModelIndex &index, /**< Индекс элемента */
							int role /**< Роль */
							) const;
		/** @brief Установить данные элемента
		 *	@brief @return Успешность выполнения операции
		 * */
		bool setData(const QModelIndex & index, /**< Индекс элемента */
					const QVariant & value, /**< Сохраняемое значение */
					int role = Qt::EditRole /**< Роль */
					);

		/** @brief Получить флаги элемента
		 *	@brief @return Флаги элемента
		 * */
		Qt::ItemFlags flags(const QModelIndex &index /**< Индекс элемента */) const;

		/** @brief Получить список типов MIME, которые могут быть использованы для описания индексов
		 *	@brief @return Список типов MIME, которые могут быть использованы для описания индексов
		 * */
		QStringList mimeTypes () const;
		/** @brief Получить сериализованное представление объекта для операции drag'n'drop
		 *	@brief @return Сериализованное представление объекта для операции drag'n'drop
		 * */
		QMimeData * mimeData ( const QModelIndexList & indexes ) const;
		/** @brief Получить операции drag'n'drop, поддерживаемые моделью
		 *	@brief @return Операции drag'n'drop, поддерживаемые моделью
		 * */
		Qt::DropActions supportedDropActions () const;
		/** @brief Обработать событие drag'n'drop
		 *	@brief @return Успешность выполнения операции
		 * */
		bool dropMimeData(const QMimeData *data, /**< Данные события */
							Qt::DropAction action, /**< Действие */
							int row, /**< Номер строки */
							int column, /**< Номер столбца */
							const QModelIndex &parent /**< Индекс родительского элемента */
							);

		/** @brief Удалить строки модели
		 *	@brief @return Успешность выполнения операции
		 * */
		bool removeRows ( int row, /**< Номер первой удаляемой строки */
						int count, /**< Число удаляемых строк */
						const QModelIndex & parent = QModelIndex() /**< Индекс родительского элемента */
						);

		/** @brief Получить число рядов данного элемента
		 *	@brief @return Число рядов данного элемента
		 * */
		int rowCount(const QModelIndex &parent = QModelIndex() /**< Индекс элемента */) const;
		/** @brief Получить число колонок данного элемента
		 *	@brief @return Число колонок данного элемента
		 * */
		int columnCount(const QModelIndex &parent = QModelIndex() /**< Индекс элемента */) const;

		/** @brief Получить код последней ошибки
		 *	@brief @return Код последней ошибки
		 * */
		int getLastError()
		{
			return m_error;
		}

		/** @brief Получить состояние клиента репозитория
		 *	@brief @return Состояние клиента репозитория
		 * */
		int getState()
		{
			return repoClient->state();
		}

		/** @brief Undo-safe сохраниение данных элемента
		 * */
		void safeSetData(const QModelIndex & index, /**< Индекс элемента */
					const QVariant & value, /**< Сохраняемое значение */
					int role = Qt::EditRole /**< Роль */
		);

		/** @brief Создать корневой элемент диаграммы, чтобы юзеру не пришлось
		 * тащить krnnDiagram из палитры вручную.
		 * */
		QModelIndex createDefaultTopLevelItem();

		/** @brief Получить индекс категории krnnDiagram. Обычно используется,
		 * чтобы сделать его корневым элементом.
		 * */
		QModelIndex getDiagramCategoryIndex() const;

		void changeParent(QPersistentModelIndex elem,QPersistentModelIndex newParent, QPointF newPos);

	public slots:
		/** @brief Отменить последнее действие */
		void undo();
		/** @brief Повторить последнее отмененное действие */
		void redo();
		/** @brief Показать окно Command list */
		void showCommandList();

	signals:
		void canUndoChanged (bool canUndo);
		void canRedoChanged (bool canRedo);

	private:
		/** @brief Клиент репозитория */
		RealRepoClient *repoClient;

		/** @brief Элемент иерерхической структуры модели */
		struct RepoTreeItem {
			IdType id; /**< Идентификатор */
			int row; /**< Номер строки */
			RepoTreeItem *parent; /**< Родительский элемент */
			QList<RepoTreeItem *> children; /**< Список дочерних элементов */
		};

		/** @brief Тип элемента */
		enum ElementType { Root, /**< Корневой элемент*/
						Category, /**< Тип */
						Container /**< Контейнер */
						};

		typedef enum CopyType { FULL_COPY_TYPE,
								SYM_LINK_TYPE

		} CopyType;

		/** @brief Получить индекс элемента модели
		 *	@brief @return Индекс элемента модели
		 * */
		QModelIndex index(const RepoTreeItem *item /**< Элемент */) const;

		/** @brief Хэш имен элементов */
		QHash<IdType, QString> hashNames;
		/** @brief Хэш типов элементов */
		QHash<IdType, TypeIdType> hashTypes;

		/** @brief Хэш значений свойств элементов */
		QHash<IdType, QMap<int, QVariant> > hashElementProps;

		/** @brief Хэш элементов иерархической структуры модели */
		QHash<IdType, QList<RepoTreeItem *> > hashTreeItems;

		/** @brief Параметры элемента на диаграмме */
		struct ElementOnDiagram {
			QPoint position; /**< Расположение */
			QPolygon configuration; /**< Конфигурация */
			ElementOnDiagram operator=(ElementOnDiagram operand) {
				position = operand.position;
				configuration = operand.configuration;
				return *this;
			}
		};

		/** @brief Хэш элементов диаграмм */
		QHash<IdType, QMap<IdType, ElementOnDiagram> > hashDiagramElements;

		/** @brief Очистить внутренние иерархические структуры элементов
		 * */
		void cleanupTree(RepoTreeItem *root);

		/** @brief Обновить свойства элемента
		 * */
		void updateProperties(IdType const &id);

		/** @brief Обновить корневой элемент
		 * */
		void updateRootTable();

		/** @brief Создать новый элемент
		 * */
		void createItem(RepoTreeItem * item, /**< Элемент */
						IdType const &id, /**< Идентификатор */
						TypeIdType const &type /**< Тип */
						);
		/** @brief Создать ссылку на уже имеющийся элемент
		 * */
		RepoTreeItem* createItem(RepoTreeItem * item, /**< Элемент */
						IdType const &id, /**< Идентификатор */
						TypeIdType const &type, /**< Тип */
						QString name /**< Имя */
						);

		/** @brief Перечитать данные корневого элемента
		 * */
		bool readRootTable();
		/** @brief Перечитать данные о типе элементов
		 * */
		void readCategoryTable(RepoTreeItem *root);
		/** @brief Перечитать данные о диаграмме
		 * */
		void readContainerTable(RepoTreeItem *root);

		/** @brief Получить тип элемента
		 *	@brief @return Тип элемента
		 * */
		ElementType type(const RepoTreeItem *item) const;
		/** @brief Получить тип элемента
		 *	@brief @return Тип элемента
		 * */
		ElementType type(const QModelIndex &index) const;

		/** @brief Создать новый элемент и добавить его в модель
		 *	@brief @return true, если элемент добавлен, иначе false
		 * */
		bool addElementToModel(RepoTreeItem *const parentItem, const QModelIndex &parent,
			IdType const &oldParent, IdType const &newid, TypeIdType const &newtype, 
			QString const &name, QPointF const &newPos, Qt::DropAction action, int oldRow);

		/** @brief Найти порядковый номер типа в корневой таблице по id. */
		unsigned findIndex(TypeIdType const &id) const;

		/** @brief Корневой элемент */
		RepoTreeItem *rootItem;

		/** @brief Информация о типах репозитория */
		RealRepoInfo info;

		/** @brief Код последней ошибки */
		int m_error;

		/** @brief Стэк для отката действий над элементами */
		QUndoStack *undoStack;

		/** @brief Представление для отображения действий над элементами */
		QUndoView *undoView;

		/** @brief Вспомогательная переменная для организации undo-стэка */
		bool addToStack;

		/** @brief Функция для прогона тестовых запросов к репозиторию */
		void runTestQueries();

		void readItems();

		void readItemsRecurse(RepoTreeItem *parent);

		RepoTreeItem* commonCreateItem(RepoTreeItem *parentItem, IdType const &id, TypeIdType const &type);

		void removeChildrenRows(QPersistentModelIndex parent, RepoTreeItem* parentItem, int row, int count);

		RepoTreeItem* copyElement(RepoTreeItem *item, QPersistentModelIndex newParent, RepoTreeItem *parentItem);

		void copyChildren(RepoTreeItem *item, QPersistentModelIndex newElem, RepoTreeItem *newItem);

		void removeChildren(QPersistentModelIndex elem,RepoTreeItem* item);

		void readChildren(RepoTreeItem *item);
};
