/** @file editorview.h
 * 	@brief Класс, реализующий представление в схеме Model/View
 * */
#pragma once

#include <QGraphicsView>

#include "editorviewscene.h"
#include "editorviewmviface.h"

namespace qReal {

	class EditorViewMViface;
	class MainWindow;

	/** @class EditorView
	 * 	@brief Класс, реализующий интерфейс представления в схеме Model/View
	 * */
	class EditorView : public QGraphicsView
	{
		Q_OBJECT

	public:
		explicit EditorView(QWidget *parent = NULL);
		~EditorView();

		/** @brief Получить указатель на объект, реализующий интерфейс представления
		 * */
		EditorViewMViface * mvIface()
		{
			return mMVIface;
		}

		void setMainWindow(qReal::MainWindow *mainWindow);

	public slots:
		/** @brief Включить/выключить сглаживание */
		void toggleAntialiasing(bool);
		/** @brief Включить/выключить поддержку OpenGL */
		void toggleOpenGL(bool);
		/** @brief Увеличить масштаб сцены */
		void zoomIn();
		/** @brief Уменьшить масштаб сцены */
		void zoomOut();

	private:
		/** @brief Объект, реализующий интерфейс представления */
		EditorViewMViface *mMVIface;
		EditorViewScene *mScene;
	};

}
