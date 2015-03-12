#include "rotateItem.h"

using namespace graphicsUtils;

RotateItem::RotateItem(QGraphicsItem *parent)
	: AbstractItem(parent)
{
}

QRectF RotateItem::rect() const
{
	return boundingRect();
}

Rotater &RotateItem::rotater()
{
	return mRotater;
}

void RotateItem::init()
{
	mRotater.setMasterItem(this);
	mRotater.setVisible(false);
}

QVariant RotateItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
	if (change == QGraphicsItem::ItemSelectedChange) {
		mRotater.setVisible(theOnlySelectedRotateItem(value.toBool()));
		mRotater.setSelected(mRotater.isVisible());
	}

	return AbstractItem::itemChange(change, value);
}

bool RotateItem::theOnlySelectedRotateItem(bool thisSelected) const
{
	if (!thisSelected) {
		return false;
	}

	for (QGraphicsItem * const selectedItem : scene()->selectedItems()) {
		if (dynamic_cast<RotateItem *>(selectedItem) && selectedItem != this) {
			return false;
		}
	}

	return true;
}
