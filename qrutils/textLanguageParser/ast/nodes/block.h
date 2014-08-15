#pragma once

#include "qrutils/textLanguageParser/ast/nodes/node.h"

namespace textLanguageParser {
namespace ast {

class QRUTILS_EXPORT Block : public Node {
public:
	Block(QList<QSharedPointer<Node>> const &statements)
		: mStatements(statements)
	{
	}

	QList<QSharedPointer<Node>> children() const override
	{
		return mStatements;
	}

private:
	QList<QSharedPointer<Node>> mStatements;
};

}
}
