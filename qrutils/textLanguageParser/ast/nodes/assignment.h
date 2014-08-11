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

private:
	QSharedPointer<Expression> mVariable;
	QSharedPointer<Expression> mValue;
};

}
}
