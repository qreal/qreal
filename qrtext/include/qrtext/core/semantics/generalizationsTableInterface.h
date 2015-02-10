#pragma once

#include "qrtext/core/types/typeExpression.h"

namespace qrtext {
namespace core {

/// Interface that tells type inferrer which type is a subtype of which. Shall be implemented for concrete language.
class GeneralizationsTableInterface
{
public:
	virtual ~GeneralizationsTableInterface() {}

	/// Shall return true if "specific" type is a subtype of "general" type, including case when they are equivalent.
	virtual bool isGeneralization(const QSharedPointer<types::TypeExpression> &specific
			, const QSharedPointer<types::TypeExpression> &general) const = 0;
};

}
}
