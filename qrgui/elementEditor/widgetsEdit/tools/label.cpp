#include "label.h"

using namespace qReal::widgetsEdit;

Label::Label(ToolController *controller)
	: Frame(new LabelWidget("label"), controller)
{
	mLabel = dynamic_cast<LabelWidget *>(widget());
	mIcon = QIcon(":/icons/widgetsEditor/label.png");
	mTitle = "Label";
	mLabel->setGeometry(0, 0
		, LABEL_DEFAULT_WIDTH
		, LABEL_DEFAULT_HEIGHT);
}

Qt::Alignment Label::alignment() const
{
	return mLabel->alignment();
}

int Label::indent() const
{
	return mLabel->indent();
}

int Label::margin() const
{
	return mLabel->margin();
}

bool Label::openExternalLinks() const
{
	return mLabel->openExternalLinks();
}

bool Label::scaledContents() const
{
	return mLabel->hasScaledContents();
}

QString Label::text() const
{
	return mLabel->text();
}

Qt::TextFormat Label::textFormat() const
{
	return mLabel->textFormat();
}

bool Label::wordWrap() const
{
	return mLabel->wordWrap();
}

void Label::setAlignment(Qt::Alignment alignment)
{
	mLabel->setAlignment(alignment);
}

void Label::setIndent(int indent)
{
	mLabel->setIndent(indent);
}

void Label::setMargin(int margin)
{
	mLabel->setMargin(margin);
}

void Label::setOpenExternalLinks(bool open)
{
	mLabel->setOpenExternalLinks(open);
}

void Label::setScaledContents(bool isScaled)
{
	mLabel->setScaledContents(isScaled);
}

void Label::setText(QString const &text)
{
	mLabel->setText(text);
}

void Label::setTextFormat(Qt::TextFormat format)
{
	mLabel->setTextFormat(format);
}

void Label::setWordWrap(bool hasWrapping)
{
	mLabel->setWordWrap(hasWrapping);
}

LabelWidget::LabelWidget(const QString &text)
	: QLabel(text), PropertyEditor(this)
{
}

void LabelWidget::setPropertyValue(const QString &value)
{
	setText(value);
}
