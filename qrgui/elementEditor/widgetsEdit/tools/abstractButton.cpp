#include "abstractButton.h"

using namespace qReal::widgetsEdit;

AbstractButton::AbstractButton(QAbstractButton *button
		, ToolController *controller)
	: Tool(button, controller)
{
	mAbstractButton = button;
}

bool AbstractButton::hasAutoRepeat() const
{
	return mAbstractButton->autoRepeat();
}

int AbstractButton::autoRepeatDelay() const
{
	return mAbstractButton->autoRepeatDelay();
}

int AbstractButton::autoRepeatInterval() const
{
	return mAbstractButton->autoRepeatInterval();
}

QString AbstractButton::text() const
{
	return mAbstractButton->text();
}

void AbstractButton::setAutoRepeat(bool autoRepeat)
{
	mAbstractButton->setAutoRepeat(autoRepeat);
}

void AbstractButton::setAutoRepeatDelay(int autoRepeatDelay)
{
	mAbstractButton->setAutoRepeatDelay(autoRepeatDelay);
}

void AbstractButton::setAutoRepeatInterval(int autoRepeatInterval)
{
	mAbstractButton->setAutoRepeatInterval(autoRepeatInterval);
}

void AbstractButton::setText(QString const &text)
{
	mAbstractButton->setText(text);
}
