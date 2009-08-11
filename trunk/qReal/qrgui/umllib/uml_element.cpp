/** @file uml_element.cpp
 * 	@brief Базовый класс элемента на диаграмме
 * */
#include <QtGui>

#include "uml_element.h"

using namespace qReal;
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
//    updateData();

	// this should probably be here, uuid is too important...
	m_uuid = dataIndex.data(roles::idRole).value<Id>();

	update();
}

IdType Element::uuid() const
{
	return m_uuid;
}

void Element::updateData()
{
	setToolTip(dataIndex.data(Qt::ToolTipRole).toString());
}
