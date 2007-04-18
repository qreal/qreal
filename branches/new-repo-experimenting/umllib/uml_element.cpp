#include <QtGui>

#include "uml_element.h"

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
    m_uuid = dataIndex.sibling(dataIndex.row(),0).data().toInt();
    
    
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
