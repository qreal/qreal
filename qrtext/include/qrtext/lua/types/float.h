#pragma once

#include "qrtext/lua/types/number.h"

namespace qrtext {
namespace lua {
namespace types {

/// Represents floating point number type.
class Float : public Number
{
	QString toString() const override
	{
		return QObject::tr("float");
	}
};

}
}
}
