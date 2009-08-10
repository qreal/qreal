/** @file editorscene.h
 * 	@brief Сцена для отрисовки объектов
 * */
#pragma once

#include <QGraphicsScene>
#include <QAbstractItemView>
#include <QAbstractItemModel>

#include "uml_element.h"
#include "../common/classes.h"
#include "uml_nodeelement.h"

// Just forward declaration
namespace EditorScenePrivate{
	class EditorViewMViface;
};

/** @class EditorScene
 *	@brief Сцена для отрисовки объектов
 * */
class EditorScene : public QGraphicsScene
{
	Q_OBJECT

public:
	/** @brief Конструктор  */
	EditorScene(QObject *parent = 0 /**< Родительский объект*/);

	/** @brief Деструктор */
	~EditorScene();

	/** @brief Установить модель */
	void setModel(QAbstractItemModel *model /**< Модель */);

	void updateLinks();

	// Hack
	UML::Element* getElem(IdType &id) { return 0; }

protected:
	/** @brief Обработать начало события drag'n'drop */
	void dragEnterEvent( QGraphicsSceneDragDropEvent *event /**< Событие */ );
#if 1
	/** @brief Обработать перемещение элемента при drag'n'drop */
	void dragMoveEvent( QGraphicsSceneDragDropEvent *event /**< Событие */);
	/** @brief Обработать завершение события drag'n'drop */
	void dragLeaveEvent( QGraphicsSceneDragDropEvent *event /**< Событие */);
#endif
	/** @brief Обработать событие drag'n'drop */
	void dropEvent ( QGraphicsSceneDragDropEvent *event /**< Событие */);

	/** @brief Обработать событие нажатия клавиши */
	void keyPressEvent( QKeyEvent *event /**< Событие */);

	/** @brief Обработать событие нажатия кнопок мыши */
	void mousePressEvent( QGraphicsSceneMouseEvent *event /**< Событие */);

	/** @brief Обработать событие нажатия кнопок мыши */
	void mouseDoubleClickEvent( QGraphicsSceneMouseEvent *event /**< Событие */);

public slots:
	/** @brief Установить индекс корневого элемента представления */
	void setRootIndex(const QModelIndex &index /**< Индекс */);

private:
	// Emulate multiple inheritance
	// We need EditorScene : public QGraphicsScene, public QAbstractItemView
	EditorScenePrivate::EditorViewMViface *mv_iface;
};

// Qt forbids us to use Q_OBJECT nested classes.
// One of proposed solution is to use namespaces instead.
namespace EditorScenePrivate {
/** @class EditorViewMViface
 * 	@brief Класс, реализующий интерфейс представления в схеме Model/View
 * */
class EditorViewMViface : public QAbstractItemView
{
	Q_OBJECT

public:
	/** @brief Конструктор */
	EditorViewMViface(EditorScene *scene /**< Сцена для отрисовки элементов */);

	/** @brief Получить область, занимаемую объектом с данным индексом
	 *	@brief @return Область, занимаемая объектом с данным индексом
	 * */
	QRect visualRect(const QModelIndex &index /**< Индекс элемента в модели */) const {return QRect();}
	/** @brief Отобразить участок сцены, на котором расположен данный элемент */
	void scrollTo(const QModelIndex &index, /**< Индекс элемента в модели */
		      ScrollHint hint = EnsureVisible /**< Способ отображения элемента */
		     ){}
	/** @brief Получить индекс элемента, расположенного в данной точке сцены
	 *	@brief @return Индекс элемента
	 * */
	QModelIndex indexAt(const QPoint &point /**< Точка сцены */) const {return QModelIndex();}

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
	void rowsAboutToBeRemoved(const QModelIndex & parent, /**< Индекс элемента модели, ряды которого удаляются */
				  int start, /**< Номер первого удаляемого ряда */
				  int end /**< Номер последнего удаляемого ряда */
				 );
	/** @brief обработать добавление рядов в модели */
	void rowsInserted(const QModelIndex & parent, /**< Индекс элемента модели, к которому добавляются ряды */
			  int start, /**< Номер первого добавленного ряда */
			  int end /**< Номер последнего добавленного ряда */
			 );
	/** @brief Изменить положение курсора
	 *	@brief @return Индекс модели
	 * */
	QModelIndex moveCursor(QAbstractItemView::CursorAction cursorAction, /**< Действие курсора */
			       Qt::KeyboardModifiers modifiers /**< Модификаторы */
			      ) {return QModelIndex();}

	/** @brief Получить горизонтальное смещение представления
	 *	@brief @return Горизонтальное смещение представления
	 * */
	int horizontalOffset() const {return 0;}
	/** @brief Получить вертикальное смещение представления
	 *	@brief @return Вертикальное смещение представления
	 * */
	int verticalOffset() const {return 0;}

	/** @brief Узнать, является ли элемент с заданным индексом невидимым
	 *	@brief @return Является ли элемент с заданным индексом невидимым
	 * */
	bool isIndexHidden(const QModelIndex &index /**< Индекс элемента */) const {return false;}

	/** @brief Установить выделение элементов */
	void setSelection(const QRect& rect, /**< Область сцены*/
			  QItemSelectionModel::SelectionFlags command /**< Тип выделения */
			 ){}

	/** @brief Возвращает регион, в который попадают выделенные элементы
	 *	@brief @return Регион, в который попадают выделенные элементы
	 * */
	QRegion visualRegionForSelection(const QItemSelection &selection /**< Выделение */ ) const {return QRegion();}

private:
	/** @brief Сцена */
	EditorScene *scene;
	/** @brief Элементы на сцене */
	QMap<QPersistentModelIndex, UML::Element*> items;
};
};
