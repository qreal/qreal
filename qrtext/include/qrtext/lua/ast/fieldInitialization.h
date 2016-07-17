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

/// Represents field initialization in table constructor.
class QRTEXT_EXPORT FieldInitialization : public Node
{
public:
	/// Constructor for initialization in form of "key = value", like "x = 1" or "[30] = 23".
	/// @param key - table key.
	/// @param value - value to assign to a given key.
	FieldInitialization(QSharedPointer<Expression> const &key
			, QSharedPointer<Expression> const &value)
		: mKey(key), mValue(value), mImplicitKey(false)
	{
	}

	/// Constructor for initialization in form of "value", like "1" or "f(x)".
	/// @param value - value to assign to "current" key (see http://www.lua.org/work/doc/manual.html#3.4.9).
	FieldInitialization(QSharedPointer<Expression> const &value)
		: mValue(value), mImplicitKey(true)
	{
	}

	/// Returns initializer key, or null if key is implicit.
	QSharedPointer<Expression> key() const
	{
		return mKey;
	}

	/// Returns initializer value.
	QSharedPointer<Expression> value() const
	{
		return mValue;
	}

	/// Returns whether the key was implicitly or explicitly specified.
	bool implicitKey() const
	{
		return mImplicitKey;
	}

	QList<QSharedPointer<Node>> children() const override
	{
		return {mKey, mValue};
	}

private:
	void accept(core::AstVisitorInterface &visitor, const QSharedPointer<Node> &pointer
			, const QSharedPointer<Node> &parent) override
	{
		static_cast<LuaAstVisitorInterface *>(&visitor)->visit(
				qSharedPointerCast<FieldInitialization>(pointer), parent);
	}

	QSharedPointer<Expression> mKey;
	QSharedPointer<Expression> mValue;
	bool mImplicitKey;
};

}
}
}
