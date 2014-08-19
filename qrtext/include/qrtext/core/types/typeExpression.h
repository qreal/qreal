#pragma once

#include <QtCore/QList>
#include <QtCore/QSharedPointer>

#include "qrtext/declSpec.h"

namespace qrtext {
namespace core {
namespace types {

/// Base class for all type expressions.
class QRTEXT_EXPORT TypeExpression {
public:
	virtual ~TypeExpression() {}

	template<typename Type>
	bool is() const
	{
		return dynamic_cast<Type const * const>(this) != nullptr;
	}
};

}

inline QSharedPointer<types::TypeExpression> wrap(types::TypeExpression *type)
{
	return QSharedPointer<types::TypeExpression>(type);
}

}
}
