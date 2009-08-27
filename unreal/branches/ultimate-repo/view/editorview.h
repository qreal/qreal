/** @file editorview.h
 * 	@brief Класс, реализующий представление в схеме Model/View 
 */
#pragma once

#include <QtGui/QGraphicsView>

/** @class EditorView
 * 	@brief Класс, реализующий интерфейс представления в схеме Model/View
 * */
class EditorView : public QGraphicsView
{
	Q_OBJECT

public:
	/** @brief Конструктор */
	EditorView(QWidget *parent = 0 /**< Родительский объект */);
	/** @brief Деструктор */
	~EditorView();

public slots:
	/** @brief Увеличить масштаб сцены */
	void zoomIn();
	/** @brief Уменьшить масштаб сцены */
	void zoomOut();
};
