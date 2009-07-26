/** @file editorviewmviface.h
 * 	@brief Класс, реализующий интерфейс представления в схеме Model/View
 * */
#pragma once

#include <QAbstractItemView>

//#include "../kernel/definitions.h"

class QGraphicsItem;

class EditorView;
class EditorViewScene;

namespace UML {
	class Element;
}

/** @class EditorViewMViface
 * 	@brief Класс, реализующий интерфейс представления в схеме Model/View
 * */
class EditorViewMViface : public QAbstractItemView
{
	Q_OBJECT

public:
	/** @brief Конструктор */
	EditorViewMViface(EditorView *view, /**< Объект, реализующий представление */
					EditorViewScene *scene /**< Сцена для отрисовки элементов */
					);

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

	/** @brief Получить элемент с заданным идентификатором
	 *	@brief @return Элемент
	* */
//	UML::Element* getItem(IdType const &uuid /**< Идентификатор элемента */);
	EditorViewScene *scene() const;

public slots:
	/** @brief Очистить сцену */
	void reset();
	/** @brief Установить индекс корневого элемента представления */
	void setRootIndex(const QModelIndex &index /**< Индекс */);

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
//    void selectionChanged ( const QItemSelection & selected, const QItemSelection & deselected );

protected:
//    bool edit(const QModelIndex &index, EditTrigger trigger, QEvent *event);

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
//    int rows(const QModelIndex &index = QModelIndex()) const;

//    void clearScene();
//    void dumpStuff(const QModelIndex &idx);

//    int margin;
//    int totalSize;
//    int pieSize;
//    int validItems;
//    double totalValue;
//    QPoint origin;

	/** @brief Сцена */
	EditorViewScene *mScene;
	/** @brief Представление */
	EditorView *view;

	/** @brief Элементы на сцене */
	QMap<QPersistentModelIndex, UML::Element*> items;
};

