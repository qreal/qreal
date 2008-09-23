/** @file editorviewscene.h
 * 	@brief Сцена для отрисовки объектов
 * */
#ifndef EDITORVIEWSCENE_H
#define EDITORVIEWSCENE_H

#include <QtGui/QGraphicsScene>

#include "uml_element.h"

class EditorViewMViface;
class EditorView;

/** @class EditorViewScene
 *	@brief Сцена для отрисовки объектов
 * */
class EditorViewScene : public QGraphicsScene
{
    Q_OBJECT

public:
	/** @brief Конструктор  */
    EditorViewScene(QObject *parent = 0 /**< Родительский объект*/);

	/** @brief Очистить сцену */
    void clearScene();
    
	/** @brief Получить элемент сцены по его идентификатору 
	 *	@brief @return Элемент сцены
	 * */
    UML::Element *getElem(int uuid /**< Идентификатор элемента */);
   	/** @brief Получить элемент сцены по его индексу в модели
	 *	@brief @return
	 * */
	UML::Element *getElemByModelIndex(const QModelIndex& index /**< Индекс элемента в модели */);
    
protected:
	/** @brief Обработать начало события drag'n'drop */
    void dragEnterEvent( QGraphicsSceneDragDropEvent *event /**< Событие */ );
	/** @brief Обработать перемещение элемента при drag'n'drop */
    void dragMoveEvent( QGraphicsSceneDragDropEvent *event /**< Событие */);
	/** @brief Обработать завершение события drag'n'drop */
    void dragLeaveEvent( QGraphicsSceneDragDropEvent *event /**< Событие */);
	/** @brief Обработать событие drag'n'drop */
    void dropEvent ( QGraphicsSceneDragDropEvent *event /**< Событие */);

	/** @brief Обработать событие нажатия клавиши */
    void keyPressEvent( QKeyEvent *event /**< Событие */);
private:
	/** @brief Новое положение элемента */
	QPointF newElementsPosition;

	/** @brief Объект, реализующий интерфейс представления в схеме Model/View */
    EditorViewMViface *mv_iface;
	/** @brief Объект, реализующей представление в схеме Model/View */
    EditorView *view;
    
    friend class EditorViewMViface;
};

#endif
