#pragma once

#include "qrtext/core/ast/node.h"
#include "node.h"

namespace simpleParser {
namespace ast {

/// Class that representis program node.
class Program : public Node
{
public:
	/// Constructor.
	/// @param statements - a list of statements in a program.
	explicit Program(const QList<QSharedPointer<Node>> &statements)
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
