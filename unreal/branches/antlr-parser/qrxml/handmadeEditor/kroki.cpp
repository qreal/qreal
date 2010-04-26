#include "kroki.h"

#include <QtGui/QTextDocument>

using namespace UML;

Kroki::Kroki() {
	mRenderer.load(QString(":/TestEditor/Orthodox/Kroki.sdf"));
	mText = "";
	mContents.setWidth(100);
	mContents.setHeight(100);
	mTitle.setFlags(0);
	mTitle.setTextInteractionFlags(Qt::NoTextInteraction);
	mTitle.setParentItem(this);
}

void Kroki::updateData()
{
	NodeElement::updateData();
	mText = "";
	update();
}

void Kroki::paint(QPainter *painter, const QStyleOptionGraphicsItem *style, QWidget *widget)
{
	mRenderer.render(painter, mContents);
	NodeElement::paint(painter, style, widget, NULL);
}
