#include "plainTextEdit.h"

using namespace qReal::widgetsEdit;

PlainTextEdit::PlainTextEdit(ToolController *controller)
	: AbstractScrollArea(new PlainTextEditWidget, controller)
{
	mPlainTextEdit = dynamic_cast<PlainTextEditWidget *>(widget());
	mTitle = tr("Plain Text Edit");
	mTag = "PlainTextEdit";
	mIcon = QIcon(":/icons/widgetsEditor/plainTextEdit.png");
	mProxy = new PlainTextEditProxy(mPlainTextEdit);
}

PlainTextEditProxy::PlainTextEditProxy(PlainTextEditWidget *plainTextEdit)
	: AbstractScrollAreaProxy(plainTextEdit)
	, mPlainTextEdit(plainTextEdit)
{
	connect(mPlainTextEdit, SIGNAL(textChanged())
		, this, SLOT(textChanged()));
}

void PlainTextEditProxy::textChanged()
{
	// Same reason as in property editing mechanism
	if (!mIgnoreNextSignal) {
		onPropertyChanged("plainText", QVariant(mPlainTextEdit->toPlainText()));
	}
	mIgnoreNextSignal = false;
}

bool PlainTextEditProxy::backgroundVisible() const
{
	return mPlainTextEdit->backgroundVisible();
}

bool PlainTextEditProxy::centerOnScroll() const
{
	return mPlainTextEdit->centerOnScroll();
}

int PlainTextEditProxy::cursorWidth() const
{
	return mPlainTextEdit->cursorWidth();
}

bool PlainTextEditProxy::overwriteMode() const
{
	return mPlainTextEdit->overwriteMode();
}

QString PlainTextEditProxy::plainText() const
{
	return mPlainTextEdit->toPlainText();
}

bool PlainTextEditProxy::readOnly() const
{
	return mPlainTextEdit->isReadOnly();
}

bool PlainTextEditProxy::undoRedoEnabled() const
{
	return mPlainTextEdit->isUndoRedoEnabled();
}

void PlainTextEditProxy::setBackgroundVisible(bool visible)
{
	mPlainTextEdit->setBackgroundVisible(visible);
}

void PlainTextEditProxy::setCenterOnScroll(bool center)
{
	mPlainTextEdit->setCenterOnScroll(center);
}

void PlainTextEditProxy::setCursorWidth(int width)
{
	mPlainTextEdit->setCursorWidth(width);
}

void PlainTextEditProxy::setOverwriteMode(bool needOverwrite)
{
	mPlainTextEdit->setOverwriteMode(needOverwrite);
}

void PlainTextEditProxy::setPlainText(QString const &plainText)
{
	mIgnoreNextSignal = true;
	mPlainTextEdit->setPlainText(plainText);
}

void PlainTextEditProxy::setReadOnly(bool isReadOnly)
{
	mPlainTextEdit->setReadOnly(isReadOnly);
}

void PlainTextEditProxy::setUndoRedoEnabled(bool undoRedoEnabled)
{
	mPlainTextEdit->setUndoRedoEnabled(undoRedoEnabled);
}

PlainTextEditWidget::PlainTextEditWidget()
	: QPlainTextEdit(), PropertyEditor(this), mIgnoreNextSignal(false)
{
	connect(this, SIGNAL(textChanged()), this, SLOT(onTextChanged()));
}

void PlainTextEditWidget::setPropertyValue(const QVariant &value)
{
	mIgnoreNextSignal = true;
	setPlainText(value.toString());
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
