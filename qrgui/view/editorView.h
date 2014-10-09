#pragma once

#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QLayout>

#include "view/editorViewScene.h"
#include "view/private/editorViewMVIface.h"
#include "view/private/touchSupportManager.h"
#include "miniMap.h"
#include "miniMapShell.h"

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

signals:
	void changeMiniMapPos(QPoint miniMapPos);

public slots:
	void toggleAntialiasing(bool);
	void zoomIn();
	void zoomOut();
	void zoom(qreal const zoomFactor);
	void updateMiniMap();
	void moveMiniMap(QPoint const miniMapPos);

protected:
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void scrollContentsBy(int dx, int dy);

	virtual void keyPressEvent(QKeyEvent *event);
	virtual void keyReleaseEvent(QKeyEvent *event);

	virtual bool viewportEvent(QEvent *event);

private slots:
	void zoomInTime();
	void zoomOutTime();
	void animFinished();
	void adjustMinimapZoom(int zoom);

private:
	void checkGrid();

	void initMiniMap();

	void startAnimation(char const *slot);

	EditorViewMViface *mMVIface;
	EditorViewScene *mScene;

	MiniMap *mMiniMap;
	MiniMapShell *mMiniMapShell;

	QPointF mMouseOldPosition;
	bool mWheelPressed;

	view::details::TouchSupportManager mTouchManager;
};

}
