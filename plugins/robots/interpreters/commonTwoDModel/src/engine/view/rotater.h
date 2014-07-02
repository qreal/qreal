#pragma once

#include <QtWidgets/QGraphicsItem>
#include <QtGui/QPainter>

#include <qrutils/graphicsUtils/abstractItem.h>
#include <qrutils/graphicsUtils/lineImpl.h>
#include <qrutils/graphicsUtils/rotateItem.h>

namespace twoDModel {
namespace view {

const int length = 30;
const int addAngle = 30; // Degrees; for arrow pike drawing

/// Class that represents line that allows to rotate QGraphicItems
class Rotater : public QObject, public graphicsUtils::AbstractItem
{
	Q_OBJECT

public:
	Rotater();

	virtual QRectF boundingRect() const;

	virtual void drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
	virtual void drawExtractionForItem(QPainter* painter);
	virtual void drawFieldForResizeItem(QPainter* painter);
	virtual void setPenBrushForExtraxtion(QPainter* painter, const QStyleOptionGraphicsItem* option);
	virtual void resizeItem(QGraphicsSceneMouseEvent *event);
	virtual void calcResizeItem(QGraphicsSceneMouseEvent *event);

	virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);

	void setMasterItem(graphicsUtils::RotateItem *masterItem);

private:
	int mLength;
	int mDrift;
	int mResizeDrift;
	graphicsUtils::RotateItem *mMaster;
	graphicsUtils::LineImpl mLineImpl;
};

}
}
