#include <QtGui>

#include "uml_element.h"

#include "realreporoles.h"

using namespace UML;

Element::Element()
    : m_uuid(1234567), moving(false)
{
    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptDrops(true);
    setCursor(Qt::PointingHandCursor);
}

void Element::setIndex(QPersistentModelIndex &index)
{
    dataIndex = index;
//    updateData();

    // this should probably be here, uuid is too important...
    m_uuid = dataIndex.data(Unreal::IdRole).toInt();

	qDebug() << "uuid is" << m_uuid;
        
    update();
}

int Element::uuid() const
{
    return m_uuid;
}

void Element::updateData()
{
    setToolTip(dataIndex.data(Qt::ToolTipRole).toString());
}
/*
void Element::getProperty( const QString &proprety )
{
    
}
*/
