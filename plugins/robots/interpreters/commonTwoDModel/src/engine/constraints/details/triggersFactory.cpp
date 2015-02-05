#include "triggersFactory.h"

#include "event.h"

using namespace twoDModel::constraints::details;

TriggersFactory::TriggersFactory(Events &events, Variables &variables)
	: mEvents(events)
	, mVariables(variables)
{
}

#include <QDebug>
Trigger TriggersFactory::fail(const QString &message) const
{
	return [message]() { qDebug() << "Хаха, лох," << message; };
}

Trigger TriggersFactory::success() const
{
	return []() { qDebug() << "Норм посан, прошел все, уважение"; };
}

Trigger TriggersFactory::combined(const QList<Trigger> &triggers) const
{
	return [triggers]() {
		for (const Trigger &trigger : triggers) {
			trigger();
		}
	};
}

Trigger TriggersFactory::setVariable(const QString &name, const QString &value) const
{
	return [this]() { mVariables[name] = value; };
}

Trigger TriggersFactory::addToVariable(const QString &name, const QString &value) const
{
	return [this]() { mVariables[name] = value; };
}

Trigger TriggersFactory::setUpEvent(const QString &id) const
{
	return [this]() {
		if (!mEvents.contains(id)) {
			/// @todo: Show error;
		}

		mEvents[id]->setUp();
	};
}

Trigger TriggersFactory::dropEvent(const QString &id) const
{
	return [this]() {
		if (!mEvents.contains(id)) {
			/// @todo: Show error;
		}

		mEvents[id]->drop();
	};
}
