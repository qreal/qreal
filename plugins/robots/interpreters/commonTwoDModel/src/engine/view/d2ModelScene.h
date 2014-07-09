#pragma once

#include <QtWidgets/QGraphicsScene>
#include <QtGui/QPainter>

#include <qrutils/graphicsUtils/abstractScene.h>
#include <qrutils/graphicsUtils/gridDrawer.h>

namespace twoDModel {

namespace items {
class WallItem;
class LineItem;
class StylusItem;
class EllipseItem;
}

namespace view {

/// Implementation of QGraphicsScene for 2D robot model
class D2ModelScene: public graphicsUtils::AbstractScene
{
	Q_OBJECT

public:
	explicit D2ModelScene(graphicsUtils::AbstractView *view, QObject *parent = 0);

public slots:
	void addWall(bool on);
	void addLine(bool on);
	void addStylus(bool on);
	void addEllipse(bool on);
	void clearScene(bool removeRobot = false);
	void setNoneButton();
	void resetButtons();

private:
	enum DrawingAction
	{
		none = 0
		, wall
		, line
		, stylus
		, Port
		, ellipse
		, noneWordLoad
	};

	void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);

	void keyPressEvent(QKeyEvent *event) override;

	void forPressResize(QGraphicsSceneMouseEvent *event) override;
	void forMoveResize(QGraphicsSceneMouseEvent *event, QRectF const &rect);
	void forReleaseResize(QGraphicsSceneMouseEvent *event, QRectF const &rect);
	void reshapeItem(QGraphicsSceneMouseEvent *event, QRectF const &rect);
	void drawBackground(QPainter *painter, QRectF const &rect) override;

	void deleteItem(QGraphicsItem *item);

	void reshapeWall(QGraphicsSceneMouseEvent *event);
	void reshapeLine(QGraphicsSceneMouseEvent *event);
	void reshapeStylus(QGraphicsSceneMouseEvent *event);
	void reshapeEllipse(QGraphicsSceneMouseEvent *event);
	void alignWalls();

	RobotItem *mRobot;

	graphicsUtils::GridDrawer mGridDrawer;
	qreal mWidthOfGrid;

	/// Current action (toggled button on left panel)
	DrawingAction mDrawingAction;

	/// Variable to count clicks on scene, used to create walls
	int mMouseClicksCount;

	/// Temporary wall that's being created. When it's complete, it's added to world model
	items::WallItem *mCurrentWall;
	items::LineItem *mCurrentLine;
	items::StylusItem *mCurrentStylus;
	items::EllipseItem *mCurrentEllipse;

	bool mClearing;
};

}
}
