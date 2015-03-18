#pragma once

#include "node.h"

namespace simpleParser {
namespace ast {

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

	QSharedPointer<Node> identifier() const
	{
		return mIdentifier;
	}

	QSharedPointer<Node> type() const
	{
		return mType;
	}

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
		return {mIdentifier, mType, mProgram, mLinkType};
	}

private:
	QSharedPointer<Node> mIdentifier;
	QSharedPointer<Node> mType;
	QSharedPointer<Node> mProgram;
	QSharedPointer<Node> mLinkType;
};
}
}
