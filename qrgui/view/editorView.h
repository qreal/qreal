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
		void showScene();
		void toggleAntialiasing(bool);
		void toggleOpenGL(bool);
		void zoomIn();
		void zoomOut();
		void invalidateScene();

	signals:
		void sceneRectChanged(QRectF);

	protected:
		enum Direction {
			horizontal,
			vertical
		};

		virtual void mouseMoveEvent(QMouseEvent *event);
		virtual void mouseReleaseEvent(QMouseEvent *event);
		virtual void mousePressEvent(QMouseEvent *event);
		virtual void scrollContentsBy(int dx, int dy);

		virtual void keyPressEvent(QKeyEvent *event);
		virtual void keyReleaseEvent(QKeyEvent *event);

		QPointF getCenter() const;
		void setCenter(QPointF const &centerPoint);
		void moveTo(Direction direction, qreal dx);

	private:
		EditorViewMViface *mMVIface;
		EditorViewScene *mScene;
		QPoint mMouseOldPosition;
		QPointF center;
		bool mWheelPressed;
		void checkGrid();
		int mZoom;
	};

}
