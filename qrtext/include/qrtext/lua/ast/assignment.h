#pragma once

#include "qrtext/lua/ast/node.h"
#include "qrtext/lua/ast/expression.h"

namespace qrtext {
namespace lua {
namespace ast {

class QRTEXT_EXPORT Assignment : public Node {
public:
	Assignment(QSharedPointer<Expression> const &variable
			, QSharedPointer<Expression> const &value)
		: mVariable(variable), mValue(value)
	{
	}

	QSharedPointer<Expression> const &variable() const
	{
		return mVariable;
	}

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
