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
class Rotater : public graphicsUtils::AbstractItem
{
	Q_OBJECT

public:
	Rotater();

	QRectF boundingRect() const;

	void drawItem(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget* widget = 0) override;
	void drawExtractionForItem(QPainter* painter) override;
	void drawFieldForResizeItem(QPainter* painter) override;
	void setPenBrushForExtraxtion(QPainter* painter, const QStyleOptionGraphicsItem* option) override;
	void resizeItem(QGraphicsSceneMouseEvent *event) override;
	void calcResizeItem(QGraphicsSceneMouseEvent *event) override;

	void setMasterItem(graphicsUtils::RotateItem *masterItem);

private:
	void mousePressEvent(QGraphicsSceneMouseEvent * event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent * event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override;
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

	int mLength;
	int mDrift;
	int mResizeDrift;
	graphicsUtils::RotateItem *mMaster;
	graphicsUtils::LineImpl mLineImpl;
};

}
}
