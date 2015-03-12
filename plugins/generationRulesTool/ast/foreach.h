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
	explicit Foreach(QSharedPointer<Node> const &identifier, QSharedPointer<Node> const &program)
		: mIdentifier(identifier)
		, mProgram(program)
	{
	}

	/// Returns identifier node.
	QSharedPointer<Node> identifier()
	{
		return mIdentifier;
	}

	/// Returns program node.
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
	QSharedPointer<Node> mProgram;
};
}
}
