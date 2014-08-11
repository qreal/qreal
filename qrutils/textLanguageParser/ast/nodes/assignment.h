#pragma once

#include "textLanguageParser/ast/nodes/node.h"
#include "textLanguageParser/ast/nodes/expression.h"

namespace textLanguageParser {
namespace ast {

class QRUTILS_EXPORT Assignment : public Node {
public:
	Assignment(QSharedPointer<Expression> const &variable, QSharedPointer<Expression> const &value)
		: mVariable(variable), mValue(value)
	{
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
