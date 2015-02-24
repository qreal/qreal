#include "conditionsFactory.h"

#include <QtCore/QSharedPointer>

#include "event.h"

using namespace twoDModel::constraints::details;

ConditionsFactory::ConditionsFactory(Events &events
		, const Variables &variables
		, const Objects &objects
		, StatusReporter &status)
	: mEvents(events)
	, mVariables(variables)
	, mObjects(objects)
	, mStatus(status)
{
}

Condition ConditionsFactory::constant(bool value) const
{
	return [value]() { return value; };
}

Condition ConditionsFactory::combined(const QList<Condition> &conditions, Glue glue) const
{
	return [conditions, glue]() {
		for (const Condition &condition : conditions) {
			if (glue == Glue::And && !condition()) {
				return false;
			}

			if (glue == Glue::Or && condition()) {
				return true;
			}
		}

		return glue == Glue::And;
	};
}

Condition ConditionsFactory::negation(const Condition &condition) const
{
	return [condition]() { return !condition(); };
}

Condition ConditionsFactory::equals(const Value &leftValue, const Value &rightValue) const
{
	return [leftValue, rightValue]() { return leftValue() == rightValue(); };
}

Condition ConditionsFactory::notEqual(const Value &leftValue, const Value &rightValue) const
{
	return [leftValue, rightValue]() { return leftValue() != rightValue(); };
}

Condition ConditionsFactory::greater(const Value &leftValue, const Value &rightValue) const
{
	return [leftValue, rightValue]() { return leftValue() > rightValue(); };
}

Condition ConditionsFactory::less(const Value &leftValue, const Value &rightValue) const
{
	return [leftValue, rightValue]() { return leftValue() < rightValue(); };
}

Condition ConditionsFactory::notGreater(const Value &leftValue, const Value &rightValue) const
{
	return [leftValue, rightValue]() { return leftValue() <= rightValue(); };
}

Condition ConditionsFactory::notLess(const Value &leftValue, const Value &rightValue) const
{
	return [leftValue, rightValue]() { return leftValue() >= rightValue(); };
}

Condition ConditionsFactory::inside(const QString &objectId, const QString &regionId) const
{
	/// @todo:
	Q_UNUSED(objectId)
	Q_UNUSED(regionId)
	return []() { return true; };
}

Condition ConditionsFactory::settedUp(const QString &eventId) const
{
	return [eventId, this]() {
		if (mEvents.contains(eventId)) {
			reportError(QObject::tr("No such event: %1").arg(eventId));
			return false;
		}

		return mEvents[eventId]->isAlive();
	};
}

Condition ConditionsFactory::dropped(const QString &eventId) const
{
	return [eventId, this]() {
		if (mEvents.contains(eventId)) {
			reportError(QObject::tr("No such event: %1").arg(eventId));
			return true;
		}

		return !mEvents[eventId]->isAlive();
	};
}

Condition ConditionsFactory::timerCondition(int timeout, bool forceDrop, const Value &timestamp, Event &event) const
{
	// We must remember somewhere timestamp when parent event was setted up.
	// We can do this in the event itself, by why? There will be no need in it except here, so storing it in heap...
	QSharedPointer<qint64> lastSetUpTimestamp(new qint64(-1));

	QObject::connect(&event, &Event::settedUp, [lastSetUpTimestamp, timestamp]() {
		*lastSetUpTimestamp = timestamp().toLongLong();
	});

	return [timeout, forceDrop, timestamp, &event, lastSetUpTimestamp]() {
		const bool timeElapsed = *lastSetUpTimestamp >= 0 && timestamp().toLongLong() - *lastSetUpTimestamp >= timeout;
		if (timeElapsed && forceDrop) {
			// Someone may think that dropping here will not let the event fire even if all conditions are satisfied.
			// But we get into this lambda after checking this event`s aliveness, so it will fire (but after drop).
			event.drop();
		}

		return timeElapsed;
	};
}

void ConditionsFactory::reportError(const QString &message)
{
	emit mStatus.checkerError(message);
}
