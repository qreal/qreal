#pragma once

#include <QtWidgets/QGraphicsView>

#include "editor/editorViewScene.h"
#include "editor/private/editorViewMVIface.h"
#include "editor/private/touchSupportManager.h"

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

	void setDrawSceneGrid(bool show);
	void ensureElementVisible(Element const * const element);
	void ensureElementVisible(Element const * const element, int xMargin, int yMargin);
	void setTitlesVisible(bool visible);

public slots:
	void toggleAntialiasing(bool);
	void zoomIn();
	void zoomOut();
	void zoom(qreal const zoomFactor);

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

	void setSceneFont();

private:
	void checkGrid();

	void startAnimation(char const *slot);

	EditorViewMViface *mMVIface;
	EditorViewScene *mScene;
	QPointF mMouseOldPosition;
	bool mWheelPressed;
	view::details::TouchSupportManager mTouchManager;
};

}
