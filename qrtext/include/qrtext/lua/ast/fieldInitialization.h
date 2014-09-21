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

	QList<QSharedPointer<Node>> children() const override
	{
		return {mKey, mValue};
	}

private:
	QSharedPointer<Expression> mKey;
	QSharedPointer<Expression> mValue;
	bool mImplicitKey;
};

}
}
}
