#include "label.h"

using namespace qReal::widgetsEdit;

Label::Label(ToolController *controller)
	: Frame(new LabelWidget("label"), controller)
{
	mLabel = dynamic_cast<LabelWidget *>(widget());
	mTitle = tr("Label");
	mTag = "Label";
	mIcon = QIcon(":/icons/widgetsEditor/label.png");
	mProxy = new LabelProxy(mLabel);
}

LabelProxy::LabelProxy(LabelWidget *label)
	: FrameProxy(label), mLabel(label)
{
	mLabel->setGeometry(0, 0, LABEL_DEFAULT_WIDTH, LABEL_DEFAULT_HEIGHT);
}

Qt::Alignment LabelProxy::alignment() const
{
	return mLabel->alignment();
}

int LabelProxy::indent() const
{
	return mLabel->indent();
}

int LabelProxy::margin() const
{
	return mLabel->margin();
}

bool LabelProxy::openExternalLinks() const
{
	return mLabel->openExternalLinks();
}

bool LabelProxy::scaledContents() const
{
	return mLabel->hasScaledContents();
}

QString LabelProxy::text() const
{
	return mLabel->text();
}

Qt::TextFormat LabelProxy::textFormat() const
{
	return mLabel->textFormat();
}

bool LabelProxy::wordWrap() const
{
	return mLabel->wordWrap();
}

void LabelProxy::setAlignment(Qt::Alignment alignment)
{
	mLabel->setAlignment(alignment);
}

void LabelProxy::setIndent(int indent)
{
	mLabel->setIndent(indent);
}

void LabelProxy::setMargin(int margin)
{
	mLabel->setMargin(margin);
}

void LabelProxy::setOpenExternalLinks(bool open)
{
	mLabel->setOpenExternalLinks(open);
}

void LabelProxy::setScaledContents(bool isScaled)
{
	mLabel->setScaledContents(isScaled);
}

void LabelProxy::setText(QString const &text)
{
	mLabel->setText(text);
}

void LabelProxy::setTextFormat(Qt::TextFormat format)
{
	mLabel->setTextFormat(format);
}

void LabelProxy::setWordWrap(bool hasWrapping)
{
	mLabel->setWordWrap(hasWrapping);
}

LabelWidget::LabelWidget(const QString &text)
	: QLabel(text), PropertyEditor(this)
{
}

void LabelWidget::setPropertyValue(const QVariant &value)
{
	setText(value.toString());
}
