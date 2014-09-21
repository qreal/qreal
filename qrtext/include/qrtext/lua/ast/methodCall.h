#pragma once

#include "qrtext/lua/ast/expression.h"
#include "qrtext/lua/ast/identifier.h"

namespace qrtext {
namespace lua {
namespace ast {

/// Represents method call expression.
class QRTEXT_EXPORT MethodCall : public Expression
{
public:
	/// Constructor.
	/// @param object - expression which calculates to object of a method.
	/// @param methodName - name of a method to call.
	/// @param arguments - a list of actual parameters.
	MethodCall(QSharedPointer<Expression> const &object
			, QSharedPointer<Identifier> const &methodName
			, QList<QSharedPointer<Expression>> const &arguments)
		: mObject(object), mMethodName(methodName), mArguments(arguments)
	{
	}

	/// Returns object.
	QSharedPointer<Expression> const &object() const {
		return mObject;
	}

	/// Returns method identifier.
	QSharedPointer<Identifier> const &methodName() const {
		return mMethodName;
	}

	/// Returns a list of actual parameters.
	QList<QSharedPointer<Expression>> const &arguments() const {
		return mArguments;
	}

	QList<QSharedPointer<Node>> children() const override
	{
		return QList<QSharedPointer<Node>>({mObject, mMethodName}) << as<Node>(mArguments);
	}

private:
	QSharedPointer<Expression> mObject;
	QSharedPointer<Identifier> mMethodName;
	QList<QSharedPointer<Expression>> mArguments;
};

}
}
}
