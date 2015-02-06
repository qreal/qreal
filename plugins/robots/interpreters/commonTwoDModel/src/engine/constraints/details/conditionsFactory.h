#pragma once

#include "defines.h"

namespace twoDModel {
namespace constraints {
namespace details {

class ConditionsFactory
{
public:
	ConditionsFactory(Events &events, Variables &variables, const Objects &objects);

	Condition constant(bool value) const;
	Condition combined(const QList<Condition> &conditions, Glue glue) const;
	Condition negation(const Condition &condition) const;

	Condition equals(const Value &leftValue, const Value &rightValue) const;
	Condition notEqual(const Value &leftValue, const Value &rightValue) const;
	Condition greater(const Value &leftValue, const Value &rightValue) const;
	Condition less(const Value &leftValue, const Value &rightValue) const;
	Condition notGreater(const Value &leftValue, const Value &rightValue) const;
	Condition notLess(const Value &leftValue, const Value &rightValue) const;

	Condition inside(const QString &objectId, const QString &regionId) const;

	Condition settedUp(const QString &eventId) const;
	Condition dropped(const QString &eventId) const;

	Condition timerCondition(int timeout, bool forceDrop, const Value &timestamp, Event &event) const;

private:
	Events &mEvents;
	Variables &mVariables;
	const Objects &mObjects;
};

}
}
}
