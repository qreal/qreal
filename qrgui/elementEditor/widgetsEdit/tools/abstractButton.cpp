#include "abstractButton.h"

using namespace qReal::widgetsEdit;

AbstractButton::AbstractButton(QAbstractButton *button
		, ToolController *controller)
	: Tool(button, controller)
{
	mAbstractButton = button;
}

AbstractButtonProxy::AbstractButtonProxy(QAbstractButton *button)
	: ToolProxy(button), mAbstractButton(button)
{
}

bool AbstractButtonProxy::hasAutoRepeat() const
{
	return mAbstractButton->autoRepeat();
}

int AbstractButtonProxy::autoRepeatDelay() const
{
	return mAbstractButton->autoRepeatDelay();
}

int AbstractButtonProxy::autoRepeatInterval() const
{
	return mAbstractButton->autoRepeatInterval();
}

QString AbstractButtonProxy::text() const
{
	return mAbstractButton->text();
}

void AbstractButtonProxy::setAutoRepeat(bool autoRepeat)
{
	mAbstractButton->setAutoRepeat(autoRepeat);
}

void AbstractButtonProxy::setAutoRepeatDelay(int autoRepeatDelay)
{
	mAbstractButton->setAutoRepeatDelay(autoRepeatDelay);
}

void AbstractButtonProxy::setAutoRepeatInterval(int autoRepeatInterval)
{
	mAbstractButton->setAutoRepeatInterval(autoRepeatInterval);
}

void AbstractButtonProxy::setText(QString const &text)
{
	mAbstractButton->setText(text);
}
