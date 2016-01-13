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

#include "qrtext/lua/ast/node.h"
#include "qrtext/lua/ast/expression.h"

namespace qrtext {
namespace lua {
namespace ast {

/// Represents single assignment.
class Assignment : public Node
{
public:
	/// Constructor.
	/// @param variable - left-hand side expression: variable or array slice.
	/// @param value - right-hand side expression.
	Assignment(QSharedPointer<Expression> const &variable
			, QSharedPointer<Expression> const &value)
		: mVariable(variable), mValue(value)
	{
		connect(mVariable);
		connect(mValue);
	}

	/// Returns left side of an assignment.
	QSharedPointer<Expression> const &variable() const
	{
		return mVariable;
	}

	/// Returns right side of an assignment.
	QSharedPointer<Expression> const &value() const
	{
		return mValue;
	}

	QList<QSharedPointer<Node>> children() const override
	{
		return {mVariable, mValue};
	}

private:
	void accept(core::AstVisitorInterface &visitor, const QSharedPointer<Node> &pointer
			, const QSharedPointer<Node> &parent) override
	{
		static_cast<LuaAstVisitorInterface *>(&visitor)->visit(qSharedPointerCast<Assignment>(pointer), parent);
	}

	QSharedPointer<Expression> mVariable;
	QSharedPointer<Expression> mValue;
};

}
}
}
