#pragma once

#include "qrtext/core/types/typeExpression.h"

namespace qrtext {
namespace lua {
namespace types {

/// Represents string type.
class String : public core::types::TypeExpression
{
	QString toString() const override
	{
		return QObject::tr("string");
	}
};

}
}
}
