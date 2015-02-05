#pragma once

#include "defines.h"

namespace twoDModel {
namespace constraints {
namespace details {

class ConditionsFactory
{
public:
	ConditionsFactory(Events &events, Variables &variables);

	Condition combined(const QList<Condition> &conditions, Glue glue) const;

	Condition equals(const Value &leftValue, const Value &rightValue) const;
	Condition notEqual(const Value &leftValue, const Value &rightValue) const;
	Condition greater(const Value &leftValue, const Value &rightValue) const;
	Condition less(const Value &leftValue, const Value &rightValue) const;
	Condition notGreater(const Value &leftValue, const Value &rightValue) const;
	Condition notLess(const Value &leftValue, const Value &rightValue) const;

	Condition inside(const QString &objectId, const QString &regionId) const;

	Condition settedUp(const QString &eventId);
	Condition dropped(const QString &eventId);

private:
	Events &mEvents;
	Variables &mVariables;
};

}
}
}
