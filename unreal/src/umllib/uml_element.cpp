#include <QtGui>

#include "uml_element.h"

using namespace UML;

Element::Element()
    : m_uuid(1234567)
{
    setFlags(ItemIsSelectable | ItemIsMovable | ItemIsFocusable);
}

void Element::setIndex(QPersistentModelIndex &index)
{
    dataIndex = index;
    updateData();
    update();
}

int Element::uuid() const
{
    return m_uuid;
}

void Element::updateData()
{
    m_uuid = dataIndex.sibling(dataIndex.row(),0).data().toInt();
}
