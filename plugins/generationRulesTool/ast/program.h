#pragma once

#include "qrtext/core/ast/node.h"
#include "node.h"

namespace simpleParser {
namespace ast {

class Program : public Node
{
public:
	/// Constructor.
	/// @param statements - a list of statements in a program.
	explicit Program(QList<QSharedPointer<Node>> const &statements)
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
