/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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
	void accept(core::AstVisitorInterface &visitor, const QSharedPointer<Node> &pointer
			, const QSharedPointer<Node> &parent) override
	{
		static_cast<LuaAstVisitorInterface *>(&visitor)->visit(qSharedPointerCast<MethodCall>(pointer), parent);
	}

	QSharedPointer<Expression> mObject;
	QSharedPointer<Identifier> mMethodName;
	QList<QSharedPointer<Expression>> mArguments;
};

}
}
}
