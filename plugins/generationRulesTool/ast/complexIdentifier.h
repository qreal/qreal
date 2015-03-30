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
	explicit ComplexIdentifier(const QSharedPointer<Node> &firstPart, const QSharedPointer<Node> &secondPart)
		: mFirstPart(firstPart)
		, mSecondPart(secondPart)
	{
	}

	/// Returns first part of complex identifier.
	QSharedPointer<Node> firstPartOfComplexIdentifier() const
	{
		return mFirstPart;
	}

	/// Returns second part of complex identifier.
	QSharedPointer<Node> secondPartOfComplexIdentifier() const
	{
		return mSecondPart;
	}

private:
	QSharedPointer<Node> mFirstPart;
	QSharedPointer<Node> mSecondPart;
};
}
}
