#pragma once

#include <QtGui/QGraphicsView>

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
	EditorView(EditorViewScene *scene, QWidget *parent);
	~EditorView();

	EditorViewMViface * mvIface()
	{
		return mMVIface;
	}

	void setMainWindow(qReal::MainWindow *mainWindow);
	void setDrawSceneGrid(bool show);
	void ensureElementVisible(Element const * const element);

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

private:
	void init();
	void checkGrid();

	EditorViewMViface *mMVIface;
	EditorViewScene *mScene;
	QPointF mMouseOldPosition;
	bool mWheelPressed;
	int mZoom;
};

}
