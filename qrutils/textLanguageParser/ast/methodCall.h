#pragma once

#include "expression.h"
#include "identifier.h"

namespace textLanguageParser {
namespace ast {

class QRUTILS_EXPORT MethodCall : public Expression {
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

private:
	QSharedPointer<Expression> mObject;
	QSharedPointer<Identifier> mMethodName;
	QList<QSharedPointer<Expression>> mArguments;
};

}
}
