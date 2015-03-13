#pragma once

#include "regionItem.h"

namespace twoDModel {
namespace items {

/// Zone in 2D model that is drawn above some other item, maybe with some expanded bounds.
class BoundRegion : public RegionItem
{
public:
	explicit BoundRegion(const QGraphicsItem &boundItem, const QString &boundId, QGraphicsItem *parent = 0);

	/// Returns a width of the stripe on which region item wider than item it is bound to.
	int stroke() const;

	/// Sets a width of the stripe on which region item wider than item it is bound to.
	void setStroke(int stroke);

	void serialize(QDomElement &element) override;
	void deserialize(const QDomElement &element) override;

	QRectF boundingRect() const override;

private:
	QPainterPath shape() const override;
	QString regionType() const override;

	const QGraphicsItem &mBoundItem;
	const QString mBoundId;
	int mStroke;
};

}
}
