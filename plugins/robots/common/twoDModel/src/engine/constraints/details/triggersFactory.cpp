#include "triggersFactory.h"

#include "event.h"

using namespace twoDModel::constraints::details;

TriggersFactory::TriggersFactory(Events &events, Variables &variables, StatusReporter &status)
	: mEvents(events)
	, mVariables(variables)
	, mStatus(status)
{
}

Trigger TriggersFactory::doNothing() const
{
	return [](){};
}

Trigger TriggersFactory::fail(const QString &message) const
{
	return [this, message]() { emit mStatus.fail(message); };
}

Trigger TriggersFactory::success() const
{
	return [this]() { emit mStatus.success(); };
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
		if (!mVariables.contains(name)) {
			mVariables[name] = QVariant();
		}

		QVariant sum;
		/// @todo: We may add int to string, for example!
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
			reportError(QObject::tr("No such event: %1").arg(id));
			return;
		}

		mEvents[id]->setUp();
	};
}

Trigger TriggersFactory::dropEvent(const QString &id) const
{
	return [this, id]() {
		if (!mEvents.contains(id)) {
			reportError(QObject::tr("No such event: %1").arg(id));
			return;
		}

		mEvents[id]->drop();
	};
}

void TriggersFactory::reportError(const QString &message)
{
	emit mStatus.checkerError(message);
}
