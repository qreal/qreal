#pragma once

#include "qrtext/core/ast/expression.h"
#include "qrtext/lua/ast/identifier.h"

namespace qrtext {
namespace lua {
namespace ast {

class QRTEXT_EXPORT MethodCall : public core::ast::Expression {
public:
	MethodCall(QSharedPointer<Expression> const &object
			, QSharedPointer<Identifier> const &methodName
			, QList<QSharedPointer<Expression>> const &arguments)
		: mObject(object), mMethodName(methodName), mArguments(arguments)
	{
	}

	QSharedPointer<Expression> const &object() const {
		return mObject;
	}

	QSharedPointer<Identifier> const &methodName() const {
		return mMethodName;
	}

	QList<QSharedPointer<Expression>> const &arguments() const {
		return mArguments;
	}

	QList<QSharedPointer<Node>> children() const override
	{
		return QList<QSharedPointer<Node>>({mObject, mMethodName}) << as<core::ast::Node>(mArguments);
	}

private:
	QSharedPointer<Expression> mObject;
	QSharedPointer<Identifier> mMethodName;
	QList<QSharedPointer<Expression>> mArguments;
};

}
}
}
