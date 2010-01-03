/** @file uml_element.cpp
 * 	@brief Базовый класс элемента на диаграмме
 * */
#include "uml_element.h"

#include <QtGui>

using namespace qReal;
using namespace UML;

Element::Element()
	: mMoving(false)
{
	setFlags(ItemIsSelectable | ItemIsMovable | ItemClipsChildrenToShape |
	         ItemClipsToShape | ItemSendsGeometryChanges);
	setAcceptDrops(true);
	setCursor(Qt::PointingHandCursor);
}

void Element::setIndex(QPersistentModelIndex &index)
{
	mDataIndex = index;
	mUuid = mDataIndex.data(roles::idRole).value<Id>();
	update();
}

Id Element::uuid() const
{
	return mUuid;
}

void Element::updateData()
{
	setToolTip(mDataIndex.data(Qt::ToolTipRole).toString());
}

QPersistentModelIndex Element::index() const
{
	return mDataIndex;
}
