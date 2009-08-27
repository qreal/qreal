/** @file uml_element.cpp
 * 	@brief Базовый класс элемента на диаграмме
 * */
#include <QtGui>

#include "uml_element.h"

#include "realreporoles.h"

using namespace UML;

QMap<QString, int> types;

Element::Element()
	: m_uuid(""), moving(false)
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
	m_uuid = dataIndex.data(Unreal::IdRole).toString();

	QString type = dataIndex.data(Unreal::TypeRole).toString();
	if (types.contains(type)) {
		m_type = types[type];
	} else {
		m_type = UserType + types.count();
		types.insert(type, m_type);
	}

	update();
}

IdType Element::uuid() const
{
	return m_uuid;
}

int Element::type() const
{
	return m_type;
}

void Element::updateData()
{
	setToolTip(dataIndex.data(Qt::ToolTipRole).toString());
}
