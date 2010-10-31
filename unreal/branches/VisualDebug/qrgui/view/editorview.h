#pragma once

#include <QGraphicsView>

#include "editorviewscene.h"
#include "editorviewmviface.h"

namespace qReal {

	class EditorViewMViface;
	class MainWindow;

	class EditorView : public QGraphicsView
	{
		Q_OBJECT

	public:
		explicit EditorView(QWidget *parent = NULL);
		~EditorView();

		EditorViewMViface * mvIface()
		{
			return mMVIface;
		}

		void setMainWindow(qReal::MainWindow *mainWindow);
		void setDrawSceneGrid(bool show);

	public slots:
		void toggleAntialiasing(bool);
		void toggleOpenGL(bool);
		void zoomIn();
		void zoomOut();

	protected:
		virtual void mouseMoveEvent(QMouseEvent *event);
		virtual void scrollContentsBy(int dx, int dy);

	private:
		EditorViewMViface *mMVIface;
		EditorViewScene *mScene;
	};

}
