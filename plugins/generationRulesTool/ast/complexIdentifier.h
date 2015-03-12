#pragma once

#include "node.h"

namespace simpleParser {
namespace ast {

/// Class that represents identifier which contains of two parts
/// separated by dot.
class ComplexIdentifier : public Node
{
public:
	/// Constructor for ComplexIdentifier.
	/// @param firstPart - part before dot.
	/// @param secondPart - part after dot.
	explicit ComplexIdentifier(QSharedPointer<Node> const &firstPart, QSharedPointer<Node> const &secondPart)
		: mFirstPart(firstPart)
		, mSecondPart(secondPart)
	{
	}

	/// Returns first part of complex identifier.
	QSharedPointer<Node> firstPartOfComplexIdentifier()
	{
		return mFirstPart;
	}

	/// Returns second part of complex identifier.
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
