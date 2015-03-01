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

private:
	void reportError(const QString &message);

	Variables &mVariables;
	const Objects &mObjects;
	StatusReporter &mStatus;
};

}
}
}
