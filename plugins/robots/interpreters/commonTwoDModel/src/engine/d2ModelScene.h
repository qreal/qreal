#pragma once

#include <QtWidgets/QGraphicsScene>
#include <QtGui/QPainter>

#include <qrutils/graphicsUtils/abstractScene.h>
#include <qrutils/graphicsUtils/gridDrawer.h>

namespace twoDModel {

/// Implementation of QGraphicsScene for 2D robot model
class D2ModelScene: public graphicsUtils::AbstractScene
{
	Q_OBJECT
public:
	D2ModelScene(graphicsUtils::AbstractView *view, QObject *parent = 0);
	virtual ~D2ModelScene();

	void keyPressEvent(QKeyEvent *event);

	void forPressResize(QGraphicsSceneMouseEvent *event);
	void forMoveResize(QGraphicsSceneMouseEvent *event, QRectF const &rect);
	void forReleaseResize(QGraphicsSceneMouseEvent *event, QRectF const &rect);
	void reshapeItem(QGraphicsSceneMouseEvent *event, QRectF const &rect);
	void drawBackground(QPainter *painter, QRectF const &rect);

public slots:
	void updateGrid();

private:
	graphicsUtils::GridDrawer mGridDrawer;
	qreal mWidthOfGrid;

signals:
	void mousePressed(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseReleased(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseMoved(QGraphicsSceneMouseEvent *mouseEvent);
	void itemDeleted(QGraphicsItem *);

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
};


}
