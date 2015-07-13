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

namespace utils {
class TimelineInterface;
}

namespace twoDModel {
namespace constraints {
namespace details {

/// Produces functor objects that return QVariant value that can be used in event conditions.
class ValuesFactory
{
public:
	ValuesFactory(Variables &variables, const Objects &objects, StatusReporter &status);

	/// Produces functor that always returns QVariant().
	Value invalidValue() const;

	/// Produces functor that always returns bool-type QVariant with the specified value.
	Value boolValue(bool value) const;

	/// Produces functor that always returns integer-type QVariant with the specified value.
	Value intValue(int value) const;

	/// Produces functor that always returns double-type QVariant with the specified value.
	Value doubleValue(qreal value) const;

	/// Produces functor that always returns string-type QVariant with the specified value.
	Value stringValue(const QString &value) const;

	/// Produces functor that returns value of the specified variable.
	Value variableValue(const QString &name) const;

	/// Produces functor that returns Qt meta-type id of the object with the specified id.
	Value typeOf(const QString &objectId) const;

	/// Produces functor that returns Qt-property value of the object with the specified id.
	Value objectState(const QString &objectId, const QString &property) const;

	/// Produces functor that returns a number of milliseconds passed from some point (no matter what point).
	Value timestamp(const utils::TimelineInterface &timeline) const;

	/// Produces functor that returns a negated integer \a value.
	/// If \a value cannot be implicitly converted to integer 0 will be returned.
	Value unaryMinus(const Value &value) const;

	/// Produces functor that returns an absolute value of \a value.
	/// If \a value cannot be implicitly converted to numerics 0 will be returned.
	Value abs(const Value &value) const;

	/// Produces functor that returns a sum of two given integer values
	/// If values cannot be implicitly converted to integers 0 will be returned.
	Value sum(const Value &left, const Value &right) const;

	/// Produces functor that returns a difference of two given numeric values.
	/// If values cannot be implicitly converted to integers 0 will be returned.
	Value difference(const Value &left, const Value &right) const;

	/// Produces functor that returns a maximal of two given numeric values.
	/// If values cannot be implicitly converted to integers 0 will be returned.
	Value max(const Value &left, const Value &right) const;

	/// Produces functor that returns a minimal of two given numeric values.
	/// If values cannot be implicitly converted to integers 0 will be returned.
	Value min(const Value &left, const Value &right) const;

private:
	void reportError(const QString &message) const;

	Variables &mVariables;
	const Objects &mObjects;
	StatusReporter &mStatus;
};

}
}
}
