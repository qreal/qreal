#pragma once

#include <QtWidgets/QGraphicsScene>
#include <QtGui/QPainter>

#include <qrutils/graphicsUtils/abstractScene.h>
#include <qrutils/graphicsUtils/gridDrawer.h>

#include <interpreterBase/devicesConfigurationProvider.h>

namespace twoDModel {

namespace items {
class WallItem;
class LineItem;
class StylusItem;
class EllipseItem;
}

namespace model {
class Model;
}

namespace view {
class RobotItem;

/// Implementation of QGraphicsScene for 2D robot model
class D2ModelScene: public graphicsUtils::AbstractScene, public interpreterBase::DevicesConfigurationProvider
{
	Q_OBJECT

public:
	D2ModelScene(model::Model &model
			, graphicsUtils::AbstractView *view
			, QObject *parent = 0);
	~D2ModelScene() override;

public slots:
	/// Sets a flag that next user mouse actions should draw a wall on the scene.
	void addWall();

	/// Sets a flag that next user mouse actions should draw a colored line on the scene.
	void addLine();

	/// Sets a flag that next user mouse actions should draw a stylus line on the scene.
	void addStylus();

	/// Sets a flag that next user mouse actions should draw an ellipse on the scene.
	void addEllipse();

	/// Resets all drawing flags: next user mouse actions will behavior like usially.
	void setNoneStatus();

	/// Clears everything on the scene besides a robot and its sensors.
	/// @param removeRobot - if true robot graphics item will be fully recreated, its position will be nullified.
	/// @param reason - reason for scene clearing --- user action or internal needs. Depending on this we can decide
	///        whether to save changes into model.
	void clearScene(bool removeRobot, Reason reason);

	/// Aligns existing walls on the grid.
	/// @todo: Walls that do not fit on the grid must not be removed.
	void alignWalls();

	/// Returns a pointer to a robot graphics item.
	RobotItem *robot();

	/// Focuses all graphics views on the robot if it is not visible.
	void centerOnRobot();

signals:
	/// Emitted each time when user presses mouse button somewhere on the scene.
	void mousePressed();

	/// Emitted each time when user releases mouse button somewhere on the scene.
	void mouseReleased();

	/// Emitted each time when user presses mouse button on the robot item.
	void robotPressed();

private slots:
	void handleNewRobotPosition();

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

	void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

	void drawBackground(QPainter *painter, QRectF const &rect) override;
	void keyPressEvent(QKeyEvent *event) override;

	void forPressResize(QGraphicsSceneMouseEvent *event) override;
	void forMoveResize(QGraphicsSceneMouseEvent *event, QRectF const &rect);
	void forReleaseResize(QGraphicsSceneMouseEvent *event, QRectF const &rect);
	void reshapeItem(QGraphicsSceneMouseEvent *event, QRectF const &rect);

	void deleteItem(QGraphicsItem *item);

	void reshapeWall(QGraphicsSceneMouseEvent *event);
	void reshapeLine(QGraphicsSceneMouseEvent *event);
	void reshapeStylus(QGraphicsSceneMouseEvent *event);
	void reshapeEllipse(QGraphicsSceneMouseEvent *event);

	void drawInitialRobot();
	void worldWallDragged(items::WallItem *wall, QPainterPath const &shape, QRectF const &oldPos);

	model::Model &mModel;

	graphicsUtils::GridDrawer mGridDrawer;
	qreal mWidthOfGrid;

	/// Current action (toggled button on left panel)
	DrawingAction mDrawingAction;

	RobotItem *mRobot = nullptr;

	/// Temporary wall that's being created. When it's complete, it's added to world model
	items::WallItem *mCurrentWall = nullptr;
	items::LineItem *mCurrentLine = nullptr;
	items::StylusItem *mCurrentStylus = nullptr;
	items::EllipseItem *mCurrentEllipse = nullptr;
};

}
}
