#pragma once

#include "qrtext/core/types/typeExpression.h"

namespace qrtext {
namespace lua {
namespace types {

/// Reresents function type.
class Function : public core::types::TypeExpression {
public:
	Function(QSharedPointer<core::types::TypeExpression> const &returnType
			, QList<QSharedPointer<core::types::TypeExpression>> const &formalParameters)
		: mReturnType(returnType), mFormalParameters(formalParameters)
	{
	}

	QSharedPointer<core::types::TypeExpression> const &returnType() const
	{
		return mReturnType;
	}

	QList<QSharedPointer<core::types::TypeExpression>> const &formalParameters() const
	{
		return mFormalParameters;
	}

private:
	QSharedPointer<core::types::TypeExpression> mReturnType;
	QList<QSharedPointer<core::types::TypeExpression>> mFormalParameters;
};

}
}
}
