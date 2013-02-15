#include "lineEdit.h"

using namespace qReal::widgetsEdit;

LineEdit::LineEdit(ToolController *controller)
	: Tool(new LineEditWidget, controller)
{
	mLineEdit = dynamic_cast<LineEditWidget *>(widget());
	mTitle = tr("Line Edit");
	mTag = "LineEdit";
	mIcon = QIcon(":/icons/widgetsEditor/lineEdit.png");
	mProxy = new LineEditProxy(mLineEdit);
	setFocusPolicy(Qt::StrongFocus);
}

LineEditProxy::LineEditProxy(LineEditWidget *lineEdit)
	: ToolProxy(lineEdit), mLineEdit(lineEdit)
{
	connect(mLineEdit, SIGNAL(textChanged(QString))
		, this, SLOT(textChanged(QString)));
}

void LineEditProxy::textChanged(const QString &text)
{
	onPropertyChanged("text", QVariant(text));
}

Qt::Alignment LineEditProxy::alignment() const
{
	return mLineEdit->alignment();
}

QLineEdit::EchoMode LineEditProxy::echoMode() const
{
	return mLineEdit->echoMode();
}

bool LineEditProxy::frame() const
{
	return mLineEdit->hasFrame();
}

QString LineEditProxy::inputMask() const
{
	return mLineEdit->inputMask();
}

int LineEditProxy::maxLength() const
{
	return mLineEdit->maxLength();
}

QString LineEditProxy::placeholderText() const
{
	return mLineEdit->placeholderText();
}

bool LineEditProxy::readonly() const
{
	return mLineEdit->isReadOnly();
}

QString LineEditProxy::text() const
{
	return mLineEdit->text();
}

void LineEditProxy::setAlignment(Qt::Alignment alignment)
{
	mLineEdit->setAlignment(alignment);
}

void LineEditProxy::setEchoMode(QLineEdit::EchoMode echoMode)
{
	mLineEdit->setEchoMode(echoMode);
}

void LineEditProxy::setFrame(bool isFrame)
{
	mLineEdit->setFrame(isFrame);
}

void LineEditProxy::setInputMask(QString const &inputMask)
{
	mLineEdit->setInputMask(inputMask);
}

void LineEditProxy::setMaxLength(int maxLength)
{
	mLineEdit->setMaxLength(maxLength);
}

void LineEditProxy::setPlaceholderText(QString const &placeholderText)
{
	mLineEdit->setPlaceholderText(placeholderText);
}

void LineEditProxy::setReadonly(bool isReadonly)
{
	mLineEdit->setReadOnly(isReadonly);
}

void LineEditProxy::setText(QString const &text)
{
	mLineEdit->setText(text);
}

LineEditWidget::LineEditWidget()
	: QLineEdit(), PropertyEditor(this)
{
	connect(this, SIGNAL(textChanged(QString)), this, SLOT(onTextChanged(QString)));
}

void LineEditWidget::setPropertyValue(const QVariant &value)
{
	setText(value.toString());
}

void LineEditWidget::onTextChanged(const QString &text)
{
	setValueInRepo(text);
}
