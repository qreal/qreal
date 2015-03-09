#include "boundRegion.h"

#include <QtXml/QDomElement>

using namespace twoDModel::items;

int defaultStroke = 0;

BoundRegion::BoundRegion(QGraphicsItem const &boundItem, const QString &boundId, QGraphicsItem *parent)
	: RegionItem(parent)
	, mBoundItem(boundItem)
	, mBoundId(boundId)
	, mStroke(defaultStroke)
{
}

int BoundRegion::stroke() const
{
	return mStroke;
}

void BoundRegion::setStroke(int stroke)
{
	mStroke = stroke;
}

void BoundRegion::serialize(QDomElement &element)
{
	RegionItem::serialize(element);
	element.setAttribute("boundItem", mBoundId);
	element.setAttribute("stroke", mStroke);
}

void BoundRegion::deserialize(const QDomElement &element)
{
	RegionItem::deserialize(element);
	if (element.hasAttribute("stroke")) {
		bool ok = false;
		const int stroke = element.attribute("stroke").toInt(&ok);
		if (ok) {
			mStroke = stroke;
		}
	}
}

QRectF BoundRegion::boundingRect() const
{
	return mBoundItem.boundingRect().adjusted(-mStroke, -mStroke, mStroke, mStroke);
}

QPainterPath BoundRegion::shape() const
{
	if (!mStroke) {
		return mBoundItem.shape();
	}

	QPainterPathStroker stroker;
	stroker.setWidth(mStroke);
	return stroker.createStroke(mBoundItem.shape());
}
