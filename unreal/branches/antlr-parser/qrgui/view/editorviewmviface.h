/** @file editorviewmviface.h
 * 	@brief Класс, реализующий интерфейс представления в схеме Model/View
 * */
#pragma once

#include <QAbstractItemView>

class QGraphicsItem;

class EditorViewScene;

namespace UML {
	class Element;
}

namespace qReal {

	class EditorView;

	/** @class EditorViewMViface
	 * 	@brief Класс, реализующий интерфейс представления в схеме Model/View
	 * */
	class EditorViewMViface : public QAbstractItemView
	{
		Q_OBJECT

	public:
		EditorViewMViface(qReal::EditorView *view, /**< Объект, реализующий представление */
			EditorViewScene *scene /**< Сцена для отрисовки элементов */
		);

		~EditorViewMViface();

		/** @brief Получить область, занимаемую объектом с данным индексом
		 *	@brief @return Область, занимаемая объектом с данным индексом
		 * */
		QRect visualRect(const QModelIndex &index /**< Индекс элемента в модели */) const;
		/** @brief Отобразить участок сцены, на котором расположен данный элемент */
		void scrollTo(const QModelIndex &index, /**< Индекс элемента в модели */
			ScrollHint hint = EnsureVisible /**< Способ отображения элемента */
		);
		/** @brief Получить индекс элемента, расположенного в данной точке сцены
		 *	@brief @return Индекс элемента
		 * */
		QModelIndex indexAt(const QPoint &point /**< Точка сцены */) const;

		EditorViewScene *scene() const;

	public slots:
		/** @brief Очистить сцену */
		void reset();
		/** @brief Установить индекс корневого элемента представления */
		void setRootIndex(const QModelIndex &index /**< Индекс */);
		/** @brief В модели перемещены элементы и надо обновить сцену */
		void rowsAboutToBeMoved(QModelIndex const &sourceParent, int sourceStart, int sourceEnd, QModelIndex const &destinationParent, int destinationRow);
		void rowsMoved(QModelIndex const &sourceParent, int sourceStart, int sourceEnd, QModelIndex const &destinationParent, int destinationRow);

	protected slots:
		/** @brief Обработать изменение данных элемента модели */
		void dataChanged(const QModelIndex &topLeft, /**< Индекс верхнего левого элемента */
			const QModelIndex &bottomRight /**< Индекс нижнего правого элемента */
		);
		/** @brief Обработать удаление рядов из модели */
		void rowsAboutToBeRemoved ( const QModelIndex & parent, /**< Индекс элемента модели, ряды которого удаляются */
			int start, /**< Номер первого удаляемого ряда */
			int end /**< Номер последнего удаляемого ряда */
		);
		/** @brief обработать добавление рядов в модели */
		void rowsInserted ( const QModelIndex & parent, /**< Индекс элемента модели, к которому добавляются ряды */
			int start, /**< Номер первого добавленного ряда */
			int end /**< Номер последнего добавленного ряда */
		);

	protected:
		/** @brief Изменить положение курсора
		 *	@brief @return Индекс модели
		 * */
		QModelIndex moveCursor(QAbstractItemView::CursorAction cursorAction, /**< Действие курсора */
			Qt::KeyboardModifiers modifiers /**< Модификаторы */
		);

		/** @brief Получить горизонтальное смещение представления
		 *	@brief @return Горизонтальное смещение представления
		 * */
		int horizontalOffset() const;
		/** @brief Получить вертикальное смещение представления
		 *	@brief @return Вертикальное смещение представления
		 * */
		int verticalOffset() const;

		/** @brief Узнать, является ли элемент с заданным индексом невидимым
		 *	@brief @return Является ли элемент с заданным индексом невидимым
		 * */
		bool isIndexHidden(const QModelIndex &index /**< Индекс элемента */) const;

		/** @brief Установить выделение элементов */
		void setSelection(const QRect& rect, /**< Область сцены*/
			QItemSelectionModel::SelectionFlags command /**< Тип выделения */
		);

		/** @brief Возвращает регион, в который попадают выделенные элементы
		 *	@brief @return Регион, в который попадают выделенные элементы
		 * */
		QRegion visualRegionForSelection(const QItemSelection &selection /**< Выделение */ ) const;

	private:
		typedef QPair<QPersistentModelIndex, UML::Element*> IndexElementPair;

		/** @brief Сцена */
		EditorViewScene *mScene;
		/** @brief Представление */
		qReal::EditorView *mView;

		/** @brief Элементы на сцене. Индексы могут меняться ВНЕЗАПНО, так что
			использовать мапы, хеши и т.д. с ключами-индексами не получится.
			Причём, если попробовать, можно замучаться отлаживать. */
		QSet<IndexElementPair> mItems;

		UML::Element *item(QPersistentModelIndex const &index) const;
		void setItem(QPersistentModelIndex const &index, UML::Element *item);
		void removeItem(QPersistentModelIndex const &index);

		void clearItems();
	};

}
