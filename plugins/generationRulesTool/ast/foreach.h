#pragma once

#include "node.h"

namespace simpleParser {
namespace ast {

class Foreach : public Node
{
public:
	explicit Foreach(QSharedPointer<Node> const &identifier
			, QSharedPointer<Node> const &type
			, QSharedPointer<Node> const &program)
		: mIdentifier(identifier)
		, mType(type)
		, mProgram(program)
	{
	}

	QSharedPointer<Node> identifier()
	{
		return mIdentifier;
	}

	QSharedPointer<Node> type()
	{
		return mType;
	}

	QSharedPointer<Node> program()
	{
		return mProgram;
	}

	QList<QSharedPointer<Node>> children() const override
	{
		return QList<QSharedPointer<Node>>({mIdentifier, mProgram});
	}

private:
	QSharedPointer<Node> mIdentifier;
	QSharedPointer<Node> mType;
	QSharedPointer<Node> mProgram;
};
}
}
