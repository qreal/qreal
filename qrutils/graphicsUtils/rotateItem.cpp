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

void RotateItem::checkSelection()
{
	mRotater.setVisible(isSelected());
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
