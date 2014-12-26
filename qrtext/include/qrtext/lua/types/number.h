#pragma once

#include "qrtext/core/types/typeExpression.h"

namespace qrtext {
namespace lua {
namespace types {

/// General number type that has two subtypes --- integer and float.
class Number : public core::types::TypeExpression
{
	QString toString() const override
	{
		return QObject::tr("number");
	}
};

}
}
}
