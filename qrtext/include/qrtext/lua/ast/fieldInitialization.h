#pragma once

#include "qrtext/lua/ast/node.h"
#include "qrtext/lua/ast/expression.h"

namespace qrtext {
namespace lua {
namespace ast {

class QRTEXT_EXPORT FieldInitialization : public Node {
public:
	FieldInitialization(QSharedPointer<Expression> const &key
			, QSharedPointer<Expression> const &value)
		: mKey(key), mValue(value), mImplicitKey(false)
	{
	}

	FieldInitialization(QSharedPointer<Expression> const &value)
		: mValue(value), mImplicitKey(true)
	{
	}

	QSharedPointer<Expression> key() const
	{
		return mKey;
	}

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
