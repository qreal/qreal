/** @file editorview.h
 * 	@brief Класс, реализующий представление в схеме Model/View
 * */
#pragma once

#include <QGraphicsView>

#include "editorviewscene.h"
#include "editorviewmviface.h"

//class DiagramExplorerModel;
class EditorViewMViface;

namespace qReal {
	class MainWindow;
}

/** @class EditorView
 * 	@brief Класс, реализующий интерфейс представления в схеме Model/View
 * */
class EditorView : public QGraphicsView
{
	Q_OBJECT

public:
	/** @brief Конструктор */
	explicit EditorView(QWidget *parent = 0);
	/** @brief Деструктор */
	~EditorView();

	/** @brief Получить указатель на объект, реализующий интерфейс представления
	 *	@brief @return Указатель на объект, реализующий интерфейс представления
	 * */
	EditorViewMViface * mvIface()
	{
		return mv_iface;
	}

	void setMainWindow(MainWindow *mainWindow);

public slots:
	/** @brief Включить/выключить сглаживание */
	void toggleAntialiasing(bool);
	/** @brief Включить/выключить поддержку OpenGL */
	void toggleOpenGL(bool);
	/** @brief Увеличить масштаб сцены */
	void zoomIn();
	/** @brief Уменьшить масштаб сцены */
	void zoomOut();

//protected:
//    void mousePressEvent(QMouseEvent *event);

private:
	/** @brief Обхект, реализующий интерфейс представления */
	EditorViewMViface * mv_iface;
};
