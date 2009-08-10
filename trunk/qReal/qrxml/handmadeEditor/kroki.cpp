#include "kroki.h"

#include <QtGui/QTextDocument>

using namespace UML;

Kroki::Kroki() {
	mRenderer.load(QString(":/TestEditor/Orthodox/Kroki.sdf"));
	mText = "";
	m_contents.setWidth(100);
	m_contents.setHeight(100);
	d.setFlags(QGraphicsItem::ItemIsSelectable | d.flags());
	d.setTextInteractionFlags(Qt::TextEditorInteraction);
	d.setParentItem(this);
	QObject::connect(d.document(), SIGNAL(contentsChanged()), this, SLOT(changeName()));
}

void Kroki::updateData()
{
	NodeElement::updateData();
	mText = "";
	update();
}

void Kroki::paint(QPainter *painter, const QStyleOptionGraphicsItem *style, QWidget *widget)
{
	mRenderer.render(painter, m_contents);
	NodeElement::paint(painter, style, widget, NULL);
	d.setTextWidth(m_contents.width() - 15);
	d.setPos(7,0);
	d.paint(painter, style, widget);
}
