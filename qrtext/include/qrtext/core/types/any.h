#pragma once

#include "qrtext/core/types/typeExpression.h"

namespace qrtext {
namespace core {
namespace types {

/// Special type that denotes "any type" --- a type variable that is not yet constrained.
class Any : public TypeExpression
{
	QString toString() const override
	{
		return QObject::tr("any");
	}
};

}
}
}
