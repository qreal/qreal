#pragma once

#include "node.h"

namespace simpleParser {
namespace ast {

/// Class that represents foreach node.
class Foreach : public Node
{
public:
	Foreach(QSharedPointer<Node> const &identifier
			, QSharedPointer<Node> const &type
			, QSharedPointer<Node> const &program
			, QSharedPointer<Node> const &optionalLinkPart = QSharedPointer<Node>())
		: mIdentifier(identifier)
		, mType(type)
		, mProgram(program)
		, mLinkType(optionalLinkPart)
	{
	}

	/// Returns identifier node.
	QSharedPointer<Node> identifier() const
	{
		return mIdentifier;
	}

	/// Returns type node.
	QSharedPointer<Node> type() const
	{
		return mType;
	}

	/// Returns program node.
	QSharedPointer<Node> program() const
	{
		return mProgram;
	}

	QSharedPointer<Node> optionalLinkPart() const
	{
		return mLinkType;
	}

	QList<QSharedPointer<Node>> children() const override
	{
		return {mIdentifier, mProgram};
	}

private:
	QSharedPointer<Node> mIdentifier;
	QSharedPointer<Node> mType;
	QSharedPointer<Node> mProgram;
	QSharedPointer<Node> mLinkType;
};
}
}
