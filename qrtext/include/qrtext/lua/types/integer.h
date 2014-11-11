#pragma once

#include "qrtext/lua/types/number.h"

namespace qrtext {
namespace lua {
namespace types {

/// Represents integer number type.
class Integer : public Number
{
	QString toString() const override
	{
		return QObject::tr("integer");
	}
};

}
}
}
