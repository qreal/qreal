#pragma once

#include <functional>

#include "number.h"
#include "qrutils/utilsDeclSpec.h"

namespace utils {

/// Represents a number that recomputes itself each time when its value is requested
class QRUTILS_EXPORT ComputableNumber : public Number
{
public:
	typedef std::function<int()> IntComputer;

	ComputableNumber();

	explicit ComputableNumber(IntComputer const &intComputer);

	QVariant property(QString const &name) const override;
	QString toString() const override;

private:
	IntComputer mComputer;
};

}
