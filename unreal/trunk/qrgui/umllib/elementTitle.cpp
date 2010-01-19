#include "elementTitle.h"

#include <QtGui/QTextCursor>

#include "uml_nodeelement.h"
#include "uml_edgeelement.h"

using namespace UML;

ElementTitle::ElementTitle(int x, int y, QString const &text)
	: mFocusIn(false), mReadOnly(true), mBinding("")
{
	setPos(x, y);
	setHtml(text);
}

ElementTitle::ElementTitle(int x, int y, QString const &binding, bool readOnly)
	: mFocusIn(false), mReadOnly(readOnly), mBinding(binding)
{
	setPos(x, y);
}

void ElementTitle::focusOutEvent(QFocusEvent *event)
{
	QGraphicsTextItem::focusOutEvent(event);
	setTextInteractionFlags(Qt::NoTextInteraction);

	// Clear selection
	QTextCursor cursor = textCursor();
	cursor.clearSelection();
	setTextCursor(cursor);

	if (mReadOnly)
		return;

	if (mOldText != toHtml()) {
		QString value = toPlainText();
		if (mBinding == "name")
			static_cast<NodeElement*>(parentItem())->setName(value);
		else
			static_cast<NodeElement*>(parentItem())->setRoleValueByName(mBinding, value);
	}
}

void ElementTitle::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Escape)
	{
		// Restore previous text and loose focus
		setHtml(mOldText);
		clearFocus();
		return;
	}
	if (event->key() == Qt::Key_Enter ||
		event->key() == Qt::Key_Return)
	{
		// Loose focus: new name will be applied in focusOutEvent
		clearFocus();
		return;
	}
	QGraphicsTextItem::keyPressEvent(event);
}

void ElementTitle::startTextInteraction()
{
	// Already interacting?
	if (hasFocus())
		return;

	mOldText = toHtml();

	// Clear scene selection
	//if (!(event->modifiers() & Qt::ControlModifier)) - was here.
	scene()->clearSelection();
	parentItem()->setSelected(true);

	if (mReadOnly)
		setTextInteractionFlags(Qt::TextBrowserInteraction);
	else
		setTextInteractionFlags(Qt::TextEditorInteraction);
	setFocus(Qt::OtherFocusReason);

	// Set full text selection
	QTextCursor cursor = QTextCursor(document());
	cursor.select(QTextCursor::Document);
	setTextCursor(cursor);
}

void ElementTitle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	// Paint a white rectangle below text
	painter->save();
	QBrush brush(Qt::white);
	painter->setBrush(brush);
	QPen pen(Qt::transparent);
	painter->setPen(pen);
	painter->drawRect(boundingRect());
	painter->restore();

	QGraphicsTextItem::paint(painter, option, widget);
}
