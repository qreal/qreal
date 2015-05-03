#pragma once

#include "node.h"

namespace simpleParser {
namespace ast {

/// Class that represents generator node.
class Generator : public Node
{
public:
	Generator(QSharedPointer<Node> const &identifier
			, QSharedPointer<Node> const &program)
		: mIdentifier(identifier)
		, mProgram(program)
	{
	}

	/// Returns identifier node.
	QSharedPointer<Node> identifier() const
	{
		return mIdentifier;
	}

	/// Returns program node.
	QSharedPointer<Node> program() const
	{
		return mProgram;
	}

	QList<QSharedPointer<Node>> children() const override
	{
		return {mIdentifier, mProgram};
	}

private:
	QSharedPointer<Node> mIdentifier;
	QSharedPointer<Node> mProgram;
};
}
}
