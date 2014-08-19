#pragma once

#include "qrtext/core/ast/expression.h"

namespace qrtext {
namespace lua {
namespace ast {

class QRTEXT_EXPORT FunctionCall : public core::ast::Expression {
public:
	FunctionCall(QSharedPointer<Expression> const &function, QList<QSharedPointer<Expression>> const &arguments)
		: mFunction(function), mArguments(arguments)
	{
	}

	QSharedPointer<Expression>  const &function() const {
		return mFunction;
	}

	QList<QSharedPointer<Expression>> const &arguments() const {
		return mArguments;
	}

	QList<QSharedPointer<Node>> children() const override
	{
		return QList<QSharedPointer<Node>>({mFunction}) << as<core::ast::Node>(mArguments);
	}

private:
	QSharedPointer<Expression> mFunction;
	QList<QSharedPointer<Expression>> mArguments;
};

}
}
}
