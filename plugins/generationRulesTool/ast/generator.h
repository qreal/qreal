#pragma once

#include "node.h"

namespace simpleParser {
namespace ast {

/// Class that represents generator node.
class Generator : public Node
{
public:
	Generator(const QSharedPointer<Node> &identifier
			, const QSharedPointer<Node> &program)
		: mIdentifier(identifier)
		, mProgram(program)
	{
	}

	/// Returns identifier node.
	const QSharedPointer<Node> &identifier() const
	{
		return mIdentifier;
	}

	/// Returns program node.
	const QSharedPointer<Node> &program() const
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
