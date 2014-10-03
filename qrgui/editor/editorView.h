#pragma once

#include <QtWidgets/QGraphicsView>

#include "editor/private/touchSupportManager.h"
#include "editor/editorViewScene.h"
#include "editor/private/editorViewMVIface.h"

namespace qReal {

class EditorView : public QGraphicsView
{
	Q_OBJECT

public:
	EditorView(models::Models const &models, Controller &controller, Id const &rootId, QWidget *parent = 0);

	EditorViewMViface const &mvIface() const;
	EditorViewMViface &mutableMvIface();
	EditorViewScene const &editorViewScene() const;
	EditorViewScene &mutableScene();

	void setDrawSceneGrid(bool show);
	void ensureElementVisible(Element const * const element);
	void ensureElementVisible(Element const * const element, int xMargin, int yMargin);
	void setTitlesVisible(bool visible);

signals:
	/// Emitted when for some reason root element was removed and editor must be closed.
	void rootElementRemoved(QModelIndex const &rootGraphicsIndex);

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

	EditorViewScene mScene;
	EditorViewMViface mMVIface;
	QPointF mMouseOldPosition;
	bool mWheelPressed;
	view::details::TouchSupportManager mTouchManager;
};

}
