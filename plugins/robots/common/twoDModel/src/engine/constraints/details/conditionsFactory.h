/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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
	ConditionsFactory(Events &events, const Variables &variables, const Objects &objects, StatusReporter &status);

	/// Produces new condition instance that always returnes \a value.
	Condition constant(bool value) const;

	/// Produces new composite condition instance. \a conditions will be concatenated using && or ||.
	Condition combined(const QList<Condition> &conditions, Glue glue) const;

	/// Produces new condition instance that always returns negated \a condition result.
	Condition negation(const Condition &condition) const;

	/// Produces new condition instance that returnes true if \a leftValue() == \a rightValue().
	Condition equals(const Value &leftValue, const Value &rightValue) const;

	/// Produces new condition instance that returnes true if \a leftValue() != \a rightValue().
	Condition notEqual(const Value &leftValue, const Value &rightValue) const;

	/// Produces new condition instance that returnes true if \a leftValue() > \a rightValue().
	Condition greater(const Value &leftValue, const Value &rightValue) const;

	/// Produces new condition instance that returnes true if \a leftValue() < \a rightValue().
	Condition less(const Value &leftValue, const Value &rightValue) const;

	/// Produces new condition instance that returnes true if \a leftValue() <= \a rightValue().
	Condition notGreater(const Value &leftValue, const Value &rightValue) const;

	/// Produces new condition instance that returnes true if \a leftValue() >= \a rightValue().
	Condition notLess(const Value &leftValue, const Value &rightValue) const;

	/// Produces new condition instance that returnes true if item with id \a objectId lays inside of
	/// the region with id \a regionId at the call moment.
	Condition inside(const QString &objectId, const QString &regionId) const;

	/// Produces new condition instance that returnes true if the event with id \a eventId
	/// is alive at the call moment and false otherwise.
	Condition settedUp(const QString &eventId) const;

	/// Produces new condition instance that returnes true if the event with id \a eventId
	/// is not alive at the call moment and false otherwise.
	Condition dropped(const QString &eventId) const;

	/// Produces new condition instance that returnes true if \a timeout has elapsed since the last time when
	/// \a event was setted up. If event is still alive after that it will again return true.
	/// @param forceDrop If true then \a event will be dropped on the first timeout even if resulting condition
	/// is a part of combined condition and it is not satisfied at the call moment.
	Condition timerCondition(int timeout, bool forceDrop, const Value &timestamp, Event &event) const;

	/// Produces new condition that invokes the given trigger and then returns the \a returns condition result.
	/// May be useful for setting some temporal variables before taking the decision.
	Condition usingCondition(const Condition &returns, const Trigger &trigger) const;

private:
	void reportError(const QString &message) const;

	Events &mEvents;
	const Variables &mVariables;
	const Objects &mObjects;
	StatusReporter &mStatus;
};

}
}
}
