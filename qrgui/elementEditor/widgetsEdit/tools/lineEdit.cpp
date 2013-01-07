#include "lineEdit.h"

using namespace qReal::widgetsEdit;

LineEdit::LineEdit(ToolController *controller)
	: Tool(new LineEditWidget, controller)
{
	mLineEdit = dynamic_cast<LineEditWidget *>(widget());
	mTitle = tr("Line Edit");
	mTag = "LineEdit";
	mIcon = QIcon(":/icons/widgetsEditor/lineEdit.png");
	connect(mLineEdit, SIGNAL(textChanged(QString))
		, this, SLOT(textChanged(QString)));
	setFocusPolicy(Qt::StrongFocus);
}

void LineEdit::textChanged(const QString &text)
{
	emit propertyChanged("text", QVariant(text));
}

Qt::Alignment LineEdit::alignment() const
{
	return mLineEdit->alignment();
}

QLineEdit::EchoMode LineEdit::echoMode() const
{
	return mLineEdit->echoMode();
}

bool LineEdit::frame() const
{
	return mLineEdit->hasFrame();
}

QString LineEdit::inputMask() const
{
	return mLineEdit->inputMask();
}

int LineEdit::maxLength() const
{
	return mLineEdit->maxLength();
}

QString LineEdit::placeholderText() const
{
	return mLineEdit->placeholderText();
}

bool LineEdit::readonly() const
{
	return mLineEdit->isReadOnly();
}

QString LineEdit::text() const
{
	return mLineEdit->text();
}

void LineEdit::setAlignment(Qt::Alignment alignment)
{
	mLineEdit->setAlignment(alignment);
}

void LineEdit::setEchoMode(QLineEdit::EchoMode echoMode)
{
	mLineEdit->setEchoMode(echoMode);
}

void LineEdit::setFrame(bool isFrame)
{
	mLineEdit->setFrame(isFrame);
}

void LineEdit::setInputMask(QString const &inputMask)
{
	mLineEdit->setInputMask(inputMask);
}

void LineEdit::setMaxLength(int maxLength)
{
	mLineEdit->setMaxLength(maxLength);
}

void LineEdit::setPlaceholderText(QString const &placeholderText)
{
	mLineEdit->setPlaceholderText(placeholderText);
}

void LineEdit::setReadonly(bool isReadonly)
{
	mLineEdit->setReadOnly(isReadonly);
}

void LineEdit::setText(QString const &text)
{
	mLineEdit->setText(text);
}

LineEditWidget::LineEditWidget()
	: QLineEdit(), PropertyEditor(this)
{
	connect(this, SIGNAL(textChanged(QString)), this, SLOT(onTextChanged(QString)));
}

void LineEditWidget::setPropertyValue(const QString &value)
{
	setText(value);
}

void LineEditWidget::onTextChanged(const QString &text)
{
	setValueInRepo(text);
}
