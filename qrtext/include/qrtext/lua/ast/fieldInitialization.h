#pragma once

#include "qrtext/core/ast/node.h"
#include "qrtext/core/ast/expression.h"

namespace qrtext {
namespace lua {
namespace ast {

class QRTEXT_EXPORT FieldInitialization : public core::ast::Node {
public:
	FieldInitialization(QSharedPointer<core::ast::Expression> const &key
			, QSharedPointer<core::ast::Expression> const &value)
		: mKey(key), mValue(value), mImplicitKey(false)
	{
	}

	FieldInitialization(QSharedPointer<core::ast::Expression> const &value)
		: mValue(value), mImplicitKey(true)
	{
	}

	QSharedPointer<core::ast::Expression> key() const
	{
		return mKey;
	}

	QSharedPointer<core::ast::Expression> value() const
	{
		return mValue;
	}

	QList<QSharedPointer<Node>> children() const override
	{
		return {mKey, mValue};
	}

private:
	QSharedPointer<core::ast::Expression> mKey;
	QSharedPointer<core::ast::Expression> mValue;
	bool mImplicitKey;
};

}
}
}
