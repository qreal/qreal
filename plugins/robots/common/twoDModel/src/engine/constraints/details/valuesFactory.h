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

	/// Produces functor that returns value of the object with the specified address.
	/// The \a path is splitted with dots. Then the most long prefix of the path will be searched in
	/// global objects map. The remaining paths will be translated into the chain of Qt properties calls
	/// via Qt reflection.
	/// If no object found in global map or on some stage object does not contain desired property
	/// checker error will be emitted.
	Value objectState(const QString &path) const;

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

	/// Produces functor that returns a distance between two given points.
	/// If values cannot be implicitly converted to points 0 will be returned.
	Value distance(const Value &point1, const Value &point2) const;

	/// Produces functor that returns bounding rectangle of some items set.
	/// Each item should have either "boundingRect" property defined or "x" and "y".
	/// @warning: This function works O(n) where n is set size. For large items sets
	/// (like robot trace) it will work pretty slow. Caching result with <using>
	/// construction will be useful here.
	Value boundingRect(const Value &items) const;

private:
	QVariant propertyChain(const QVariant &value, const QStringList &properties, const QString &objectAlias) const;
	QVariant propertyOf(const QVariant &value, const QString &property, const QString &objectAlias) const;
	QVariant propertyOf(const QVariant &value, const QString &property
			, bool *hasProperty = 0, bool *unknownType = 0) const;
	QVariant propertyOf(const QObject *object, const QString &property, bool *ok = 0) const;
	QVariant propertyOf(const QPoint &point, const QString &property, bool *ok = 0) const;
	QVariant propertyOf(const QRect &rect, const QString &property, bool *ok = 0) const;
	QVariant propertyOf(const QVariantList &list, const QString &property, bool *ok = 0) const;
	void iterate(const QVariant &collection, const std::function<void(const QVariant &)> &visitor) const;

	void reportError(const QString &message) const;

	Variables &mVariables;
	const Objects &mObjects;
	StatusReporter &mStatus;
};

}
}
}
