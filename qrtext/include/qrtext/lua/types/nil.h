#pragma once

#include "qrtext/core/types/typeExpression.h"

namespace qrtext {
namespace lua {
namespace types {

/// Represents nil type, which has only one value.
class Nil : public core::types::TypeExpression
{
	QString toString() const override
	{
		return QObject::tr("nil");
	}
};

}
}
}
