#include <QtGui>

#include "uml_element.h"

#include "realreporoles.h"

using namespace UML;

Element::Element()
    : m_uuid(1234567), moving(false)
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
    m_uuid = dataIndex.data(Unreal::IdRole).toInt();
	m_type = dataIndex.data(Unreal::TypeRole).toInt();
        
    update();
}

int Element::uuid() const
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

