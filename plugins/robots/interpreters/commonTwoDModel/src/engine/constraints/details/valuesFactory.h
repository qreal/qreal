#pragma once

#include "defines.h"

namespace utils {
class TimelineInterface;
}

namespace twoDModel {
namespace constraints {
namespace details {

class ValuesFactory
{
public:
	ValuesFactory(Variables &variables, Objects &objects);

	Value intValue(const QString &decimalString) const;
	Value floatValue(const QString &decimalString) const;
	Value stringValue(const QString &value) const;

	Value variableValue(const QString &name) const;

	Value typeOf(const QString &objectId) const;
	Value objectState(const QString &objectId, const QString &property) const;

	Value timestamp(const utils::TimelineInterface &timeline) const;

private:
	Variables &mVariables;
	Objects &mObjects;
};

}
}
}
