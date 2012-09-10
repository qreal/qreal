#include "plainTextEdit.h"

using namespace qReal::widgetsEdit;

PlainTextEdit::PlainTextEdit(ToolController *controller)
	: AbstractScrollArea(new QPlainTextEdit, controller)
{
	mPlainTextEdit = dynamic_cast<QPlainTextEdit *>(widget());
	mIcon = QIcon(":/icons/widgetsEditor/plainTextEdit.png");
	mTitle = "Plain Text Edit";
	connect(mPlainTextEdit, SIGNAL(textChanged())
		, this, SLOT(textChanged()));
}

void PlainTextEdit::textChanged()
{
	emit propertyChanged("plainText", QVariant(mPlainTextEdit->toPlainText()));
}

bool PlainTextEdit::backgroundVisible() const
{
	return mPlainTextEdit->backgroundVisible();
}

bool PlainTextEdit::centerOnScroll() const
{
	return mPlainTextEdit->centerOnScroll();
}

int PlainTextEdit::cursorWidth() const
{
	return mPlainTextEdit->cursorWidth();
}

bool PlainTextEdit::overwriteMode() const
{
	return mPlainTextEdit->overwriteMode();
}

QString PlainTextEdit::plainText() const
{
	return mPlainTextEdit->toPlainText();
}

bool PlainTextEdit::readOnly() const
{
	return mPlainTextEdit->isReadOnly();
}

bool PlainTextEdit::undoRedoEnabled() const
{
	return mPlainTextEdit->isUndoRedoEnabled();
}

void PlainTextEdit::setBackgroundVisible(bool visible)
{
	mPlainTextEdit->setBackgroundVisible(visible);
}

void PlainTextEdit::setCenterOnScroll(bool center)
{
	mPlainTextEdit->setCenterOnScroll(center);
}

void PlainTextEdit::setCursorWidth(int width)
{
	mPlainTextEdit->setCursorWidth(width);
}

void PlainTextEdit::setOverwriteMode(bool needOverwrite)
{
	mPlainTextEdit->setOverwriteMode(needOverwrite);
}

void PlainTextEdit::setPlainText(QString const &plainText)
{
	mPlainTextEdit->setPlainText(plainText);
}

void PlainTextEdit::setReadOnly(bool isReadOnly)
{
	mPlainTextEdit->setReadOnly(isReadOnly);
}

void PlainTextEdit::setUndoRedoEnabled(bool undoRedoEnabled)
{
	mPlainTextEdit->setUndoRedoEnabled(undoRedoEnabled);
}
