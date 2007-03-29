#include <QtGui>

#include "uml_element.h"

using namespace UML;

Element::Element()
    : m_uuid(1234567)
{
    setFlags(ItemIsSelectable | ItemIsMovable | ItemIsFocusable);
}

QRectF Element::boundingRect() const
{
    return QRectF(0,0,100,100);
}

void Element::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    QPainterPath path;
    path.moveTo(0,0);
    path.lineTo(100,100);
    path.moveTo(100,0);
    path.lineTo(0,100);
    
    painter->drawPath(path);
    painter->drawRect(boundingRect());
}

void Element::setIndex(QPersistentModelIndex &index)
{
    dataIndex = index;
    updateData();
}

int Element::uuid() const
{
    return m_uuid;
}

void Element::updateData()
{
    qDebug() << "// updateData()" << m_uuid;
    m_uuid = dataIndex.sibling(dataIndex.row(),0).data().toInt();
    qDebug() << "\\\\ updateData()" << m_uuid;
}
