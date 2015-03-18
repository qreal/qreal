#pragma once

#include "node.h"

namespace simpleParser {
namespace ast {

class ComplexIdentifier : public Node
{
public:
	ComplexIdentifier(QSharedPointer<Node> const &firstPart, QSharedPointer<Node> const &secondPart)
		: mFirstPart(firstPart)
		, mSecondPart(secondPart)
	{
	}

	QSharedPointer<Node> firstPartOfComplexIdentifier()
	{
		return mFirstPart;
	}

	QSharedPointer<Node> secondPartOfComplexIdentifier()
	{
		return mSecondPart;
	}

private:
	QSharedPointer<Node> mFirstPart;
	QSharedPointer<Node> mSecondPart;
};
}
}
