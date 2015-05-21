#pragma once

#include "node.h"

namespace simpleParser {
namespace ast {

/// Class that represents condition node.
class Condition : public Node
{
public:
	/// Constructor for Condition.
	/// @param identifierPart - part defore dot.
	/// @param optionalTransitionPart - optional part after dot.
	Condition(const QSharedPointer<Node> &firstPart
			, const QSharedPointer<Node> &comparator = QSharedPointer<Node>()
			, const QSharedPointer<Node> &secondPart = QSharedPointer<Node>())
		: mFirstPart(firstPart)
		, mComparator(comparator)
		, mSecondPart(secondPart)
	{
	}

	/// Returns first part.
	const QSharedPointer<Node> &firstPart() const
	{
		return mFirstPart;
	}

	/// Returns comparator.
	const QSharedPointer<Node> &comparator() const
	{
		return mComparator;
	}

	/// Returns second part.
	const QSharedPointer<Node> &secondPart() const
	{
		return mSecondPart;
	}

private:
	QSharedPointer<Node> mFirstPart;
	QSharedPointer<Node> mComparator;
	QSharedPointer<Node> mSecondPart;
};

}
}
