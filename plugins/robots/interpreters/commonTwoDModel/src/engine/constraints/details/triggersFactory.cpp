#include "triggersFactory.h"

#include "event.h"

using namespace twoDModel::constraints::details;

TriggersFactory::TriggersFactory(Events &events, Variables &variables)
	: mEvents(events)
	, mVariables(variables)
{
}

Trigger TriggersFactory::doNothing() const
{
	return [](){};
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

Trigger TriggersFactory::setVariable(const QString &name, const QVariant &value) const
{
	return [this, name, value]() { mVariables[name] = value; };
}

Trigger TriggersFactory::addToVariable(const QString &name, const QVariant &value) const
{
	return [this, name, value]() {
		QVariant sum;
		/// @todo: We may calculate of string and int, for example!
		switch (value.type()) {
		case QVariant::Int:
			sum = mVariables[name].toInt() + value.toInt();
			break;
		case QVariant::Double:
			sum = mVariables[name].toDouble() + value.toDouble();
			break;
		default:
			sum = mVariables[name].toString() + value.toString();
			break;
		}

		mVariables[name] = sum;
	};
}

Trigger TriggersFactory::setUpEvent(const QString &id) const
{
	return [this, id]() {
		if (!mEvents.contains(id)) {
			/// @todo: Show error;
		}

		mEvents[id]->setUp();
	};
}

Trigger TriggersFactory::dropEvent(const QString &id) const
{
	return [this, id]() {
		if (!mEvents.contains(id)) {
			/// @todo: Show error;
		}

		mEvents[id]->drop();
	};
}
