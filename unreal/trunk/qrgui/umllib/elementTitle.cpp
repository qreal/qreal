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

void ElementTitle::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsTextItem::mousePressEvent(event);
	if (!(event->modifiers() & Qt::ControlModifier))
		scene()->clearSelection();
	parentItem()->setSelected(true);
	if (mFocusIn) {
		if (mReadOnly)
			setTextInteractionFlags(Qt::TextBrowserInteraction);
		else {
			QTextCursor cursor(textCursor());
			cursor.select(QTextCursor::Document);
			setTextCursor(cursor);
			mFocusIn = false;
		}
	}
}

void ElementTitle::focusInEvent(QFocusEvent *event)
{
	mFocusIn = true;
	mOldText = toHtml();
	QGraphicsTextItem::focusInEvent(event);
}

void ElementTitle::focusOutEvent(QFocusEvent *event)
{
	QGraphicsTextItem::focusOutEvent(event);
	setTextInteractionFlags(Qt::NoTextInteraction);
	if (mReadOnly)
		return;
	QString value = toPlainText();
	QString tmp = toHtml();
	// FIXME: Reset selection
	setHtml("");
	setHtml(tmp);
	if (mOldText != toHtml()) {
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

void ElementTitle::setNeededTextInteractionFlags()
{
	if (mReadOnly)
		setTextInteractionFlags(Qt::TextBrowserInteraction);
	else
		setTextInteractionFlags(Qt::TextEditorInteraction);
}
