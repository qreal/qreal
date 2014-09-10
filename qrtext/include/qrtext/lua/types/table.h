#pragma once

#include "qrtext/core/types/typeExpression.h"

namespace qrtext {
namespace lua {
namespace types {

class Table : public core::types::TypeExpression {
public:
	Table(QSharedPointer<core::types::TypeExpression> const &elementType
			, int size)
		: mElementType(elementType), mSize(size)
	{
	}

	QSharedPointer<core::types::TypeExpression> const &elementType() const
	{
		return mElementType;
	}

private:
	QSharedPointer<core::types::TypeExpression> mElementType;
	int mSize;
};

}
}
}
