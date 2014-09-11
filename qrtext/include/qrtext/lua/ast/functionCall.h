#pragma once

#include "qrtext/lua/ast/expression.h"

namespace qrtext {
namespace lua {
namespace ast {

/// Represents function call expression.
class QRTEXT_EXPORT FunctionCall : public Expression {
public:
	/// Constructor.
	/// @param function - an expression which calculates to a function to call.
	/// @param arguments - a list of actual parameters.
	FunctionCall(QSharedPointer<Expression> const &function, QList<QSharedPointer<Expression>> const &arguments)
		: mFunction(function), mArguments(arguments)
	{
	}

	/// Returns function to call.
	QSharedPointer<Expression>  const &function() const {
		return mFunction;
	}

	/// Returns list of actual parameters.
	QList<QSharedPointer<Expression>> const &arguments() const {
		return mArguments;
	}

	QList<QSharedPointer<Node>> children() const override
	{
		return QList<QSharedPointer<Node>>({mFunction}) << as<Node>(mArguments);
	}

private:
	QSharedPointer<Expression> mFunction;
	QList<QSharedPointer<Expression>> mArguments;
};

}
}
}
