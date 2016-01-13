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

namespace qrtext {
namespace lua {
namespace ast {

/// Represents function call expression.
class QRTEXT_EXPORT FunctionCall : public Expression
{
public:
	/// Constructor.
	/// @param function - an expression which calculates to a function to call.
	/// @param arguments - a list of actual parameters.
	FunctionCall(QSharedPointer<Expression> const &function, QList<QSharedPointer<Expression>> const &arguments)
		: mFunction(function), mArguments(arguments)
	{
	}

	/// Returns function to call.
	QSharedPointer<Expression> const &function() const {
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
	void accept(core::AstVisitorInterface &visitor, const QSharedPointer<Node> &pointer
			, const QSharedPointer<Node> &parent) override
	{
		static_cast<LuaAstVisitorInterface *>(&visitor)->visit(qSharedPointerCast<FunctionCall>(pointer), parent);
	}

	QSharedPointer<Expression> mFunction;
	QList<QSharedPointer<Expression>> mArguments;
};

}
}
}
