#pragma once

#include "textLanguageParser/ast/nodes/node.h"
#include "textLanguageParser/ast/nodes/expression.h"

namespace textLanguageParser {
namespace ast {

class QRUTILS_EXPORT FieldInitialization : public Node {
public:
	FieldInitialization(QSharedPointer<Expression> const &key, QSharedPointer<Expression> const &value)
		: mKey(key), mValue(value), mImplicitKey(false)
	{
	}

	FieldInitialization(QSharedPointer<Expression> const &value)
		: mValue(value), mImplicitKey(true)
	{
	}

private:
	QSharedPointer<Expression> mKey;
	QSharedPointer<Expression> mValue;
	bool mImplicitKey;
};

}
}
