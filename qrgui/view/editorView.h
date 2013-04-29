#pragma once

#include <QtWidgets/QGraphicsView>

#include "editorViewScene.h"
#include "editorViewMVIface.h"

namespace qReal {

	class EditorViewMViface;
	class MainWindow;

	class EditorView : public QGraphicsView
	{
		Q_OBJECT

	public:
		explicit EditorView(QWidget *parent);
		~EditorView();

		EditorViewMViface *mvIface() const;
		EditorViewScene *editorViewScene() const;

		void setMainWindow(qReal::MainWindow *mainWindow);
		void setDrawSceneGrid(bool show);
		void ensureElementVisible(Element const * const element);
		void ensureElementVisible(Element const * const element, int xMargin, int yMargin);
		void setTitlesVisible(bool visible);

	public slots:
		void toggleAntialiasing(bool);
		void toggleOpenGL(bool);
		void zoomIn();
		void zoomOut();
		void invalidateScene();

	protected:
		virtual void mouseMoveEvent(QMouseEvent *event);
		virtual void mouseReleaseEvent(QMouseEvent *event);
		virtual void mousePressEvent(QMouseEvent *event);
		virtual void scrollContentsBy(int dx, int dy);

		virtual void keyPressEvent(QKeyEvent *event);
		virtual void keyReleaseEvent(QKeyEvent *event);

	private:
		EditorViewMViface *mMVIface;
		EditorViewScene *mScene;
		QPointF mMouseOldPosition;
		bool mWheelPressed;
		void checkGrid();
		int mZoom;
	};

}
