#include "idWidget.h"

using namespace versioning;
using namespace versioning::details;

IdWidget::IdWidget(QString const &defaultText, QWidget *parent)
	: QWidget(parent), mDefaultText(defaultText), mIdSetted(false)
{
	mLayout = new QGridLayout(this);
	mLabel = new QLabel(mDefaultText, this);

	QFont labelFont = mLabel->font();
	labelFont.setBold(true);
	mLabel->setFont(labelFont);

	mLayout->addWidget(mLabel);
	setLayout(mLayout);
}

void IdWidget::setId(const qReal::Id &graphicalId, const qReal::Id &logicalId)
{
	mIdSetted = true;
	mGraphicalId = graphicalId;
	mLogicalId = logicalId;
	mLabel->setText(labelText());
}

void IdWidget::reset()
{
	mIdSetted = false;
	mLabel->setText(mDefaultText);
}

void IdWidget::enterEvent(QEvent *event)
{
	QWidget::enterEvent(event);
	event->accept();
	if (mIdSetted)
		emit mouseEntered(mGraphicalId);
}

void IdWidget::leaveEvent(QEvent *event)
{
	QWidget::leaveEvent(event);
	event->accept();
	if (mIdSetted)
		emit mouseLeaved(mGraphicalId);
}

QString IdWidget::labelText() const
{
	QString result = "";
	if (mLogicalId != qReal::Id())
	{
		result += "Logical Id: " + mLogicalId.toString() + "\n";
	}
	if (mGraphicalId != qReal::Id())
	{
		result += "Graphical Id: " + mGraphicalId.toString();
	}
	return result;
}
