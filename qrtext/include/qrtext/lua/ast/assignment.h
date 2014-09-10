#pragma once

#include "qrtext/lua/ast/node.h"
#include "qrtext/lua/ast/expression.h"

namespace qrtext {
namespace lua {
namespace ast {

/// Represents single assignment.
class Assignment : public Node {
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
	QSharedPointer<Expression> mVariable;
	QSharedPointer<Expression> mValue;
};

}
}
}
