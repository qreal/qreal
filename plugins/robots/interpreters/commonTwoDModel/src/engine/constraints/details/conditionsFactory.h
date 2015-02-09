#pragma once

#include "defines.h"

namespace twoDModel {
namespace constraints {
namespace details {

/// Produces functor objects that implement behaviour for tags in "condition" group.
/// Produced functors will return boolean value that means should event fire immediately or not.
/// True will be returned if the condition is satisfied in the call moment.
class ConditionsFactory
{
public:
	ConditionsFactory(Events &events, Variables &variables, const Objects &objects);

	/// Produces new condition instance that always returnes @arg value.
	Condition constant(bool value) const;

	/// Produces new composite condition instance. @arg conditions will be concatenated using && or ||.
	Condition combined(const QList<Condition> &conditions, Glue glue) const;

	/// Produces new condition instance that always returns negated @arg condition result.
	Condition negation(const Condition &condition) const;

	/// Produces new condition instance that returnes true if @arg leftValue() == @arg rightValue().
	Condition equals(const Value &leftValue, const Value &rightValue) const;

	/// Produces new condition instance that returnes true if @arg leftValue() != @arg rightValue().
	Condition notEqual(const Value &leftValue, const Value &rightValue) const;

	/// Produces new condition instance that returnes true if @arg leftValue() > @arg rightValue().
	Condition greater(const Value &leftValue, const Value &rightValue) const;

	/// Produces new condition instance that returnes true if @arg leftValue() < @arg rightValue().
	Condition less(const Value &leftValue, const Value &rightValue) const;

	/// Produces new condition instance that returnes true if @arg leftValue() <= @arg rightValue().
	Condition notGreater(const Value &leftValue, const Value &rightValue) const;

	/// Produces new condition instance that returnes true if @arg leftValue() >= @arg rightValue().
	Condition notLess(const Value &leftValue, const Value &rightValue) const;

	/// Produces new condition instance that returnes true if item with id @arg objectId lays inside of
	/// the region with id @arg regionId at the call moment.
	Condition inside(const QString &objectId, const QString &regionId) const;

	/// Produces new condition instance that returnes true if the event with id @arg eventId
	/// is alive at the call moment and false otherwise.
	Condition settedUp(const QString &eventId) const;

	/// Produces new condition instance that returnes true if the event with id @arg eventId
	/// is not alive at the call moment and false otherwise.
	Condition dropped(const QString &eventId) const;

	/// Produces new condition instance that returnes true if @arg timeout has elapsed since the last time when
	/// @arg event was setted up. If event is still alive after that it will again return true.
	/// @param forceDrop If true then @arg event will be dropped on the first timeout even if resulting condition
	/// is a part of combined condition and it is not satisfied at the call moment.
	Condition timerCondition(int timeout, bool forceDrop, const Value &timestamp, Event &event) const;

private:
	Events &mEvents;
	Variables &mVariables;
	const Objects &mObjects;
};

}
}
}
