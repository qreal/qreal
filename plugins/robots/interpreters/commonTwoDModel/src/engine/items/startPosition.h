#pragma once

#include <qrutils/graphicsUtils/rotateItem.h>

namespace twoDModel {
namespace items {

class StartPosition : public graphicsUtils::RotateItem
{
public:
	explicit StartPosition(QGraphicsItem *parent = 0);

	QRectF boundingRect() const override;
	void drawItem(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

	void serialize(QDomElement &robotElement) const;
	void deserialize(const QDomElement &robotElement) override;

private:
	void drawFieldForResizeItem(QPainter* painter) override;
	void changeDragState(qreal x, qreal y) override;
};

}
}
