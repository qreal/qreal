/** @file uml_element.cpp
 * 	@brief Базовый класс элемента на диаграмме
 * */
#include <QtGui>

#include "uml_element.h"

using namespace UML;

Element::Element()
	: moving(false)
{
	setFlags(ItemIsSelectable | ItemIsMovable | ItemClipsChildrenToShape
			| ItemClipsToShape);
	setAcceptDrops(true);
	setCursor(Qt::PointingHandCursor);
}

void Element::setIndex(QPersistentModelIndex &index)
{
	dataIndex = index;

	update();
}

void Element::updateData()
{
	setToolTip(dataIndex.data(Qt::ToolTipRole).toString());
}
