#pragma once

#include "node.h"

namespace simpleParser {
namespace ast {

/// Class that represents foreach node.
class Foreach : public Node
{
public:
	/// Constructor for Foreach.
	/// @param identifier - foreach identifier.
	/// @param program - subprogram.
	explicit Foreach(const QSharedPointer<Node> &identifier, const QSharedPointer<Node> &program)
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
