#include "plainTextEdit.h"

using namespace qReal::widgetsEdit;

PlainTextEdit::PlainTextEdit(ToolController *controller)
	: AbstractScrollArea(new PlainTextEditWidget, controller)
{
	mPlainTextEdit = dynamic_cast<PlainTextEditWidget *>(widget());
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

PlainTextEditWidget::PlainTextEditWidget()
	: QPlainTextEdit(), PropertyEditor(this), mIgnoreNextSignal(false)
{
	connect(this, SIGNAL(textChanged()), this, SLOT(onTextChanged()));
}

void PlainTextEditWidget::setPropertyValue(const QString &value)
{
	mIgnoreNextSignal = true;
	setPlainText(value);
}

void PlainTextEditWidget::onTextChanged()
{
	// Without this segfault happens (maybe signal cicling). But maybe it may
	// happen that the next signal would be suddenly user edit or smth
	// and cicling would happen anyway. But honest text versions comparison
	// would work too long. So be careful
	if (!mIgnoreNextSignal) {
		setValueInRepo(toPlainText());
	}
	mIgnoreNextSignal = false;
}
