#pragma once

#include "qrtext/core/ast/node.h"
#include "qrtext/core/ast/expression.h"

namespace qrtext {
namespace lua {
namespace ast {

class QRTEXT_EXPORT Assignment : public core::ast::Node {
public:
	Assignment(QSharedPointer<core::ast::Expression> const &variable
			, QSharedPointer<core::ast::Expression> const &value)
		: mVariable(variable), mValue(value)
	{
	}

	QSharedPointer<core::ast::Expression> const &variable() const
	{
		return mVariable;
	}

	QSharedPointer<core::ast::Expression> const &value() const
	{
		return mValue;
	}

	QList<QSharedPointer<Node>> children() const override
	{
		return {mVariable, mValue};
	}

private:
	QSharedPointer<core::ast::Expression> mVariable;
	QSharedPointer<core::ast::Expression> mValue;
};

}
}
}
