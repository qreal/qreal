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
		void changeSceneGrid();

	public slots:
		void toggleAntialiasing(bool);
		void toggleOpenGL(bool);
		void zoomIn();
		void zoomOut();

	private:
		EditorViewMViface *mMVIface;
		EditorViewScene *mScene;
	};

}
