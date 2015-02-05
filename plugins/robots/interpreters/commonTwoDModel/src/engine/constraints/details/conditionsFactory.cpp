#include "conditionsFactory.h"

#include "event.h"

using namespace twoDModel::constraints::details;

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
	return []() { return true; };
}

Condition ConditionsFactory::settedUp(const QString &eventId)
{
	return [eventId, this]() {
		if (mEvents.contains(eventId)) {
			/// @todo: Display error!
			return false;
		}

		return mEvents[eventId]->isAlive();
	};
}

Condition ConditionsFactory::dropped(const QString &eventId)
{
	return [eventId, this]() {
		if (mEvents.contains(eventId)) {
			/// @todo: Display error!
			return true;
		}

		return !mEvents[eventId]->isAlive();
	};
}
