#pragma once

#include "node.h"

namespace simpleParser {
namespace ast {

/// Class that represents condition node.
class Condition : public Node
{
public:
	/// Constructor for Condition.
	/// @param firstPart - boolean condition node.
	explicit Condition(const QSharedPointer<Node> &firstPart)
		: mFirstPart(firstPart)
		, mComparator(QSharedPointer<Node>())
		, mSecondPart(QSharedPointer<Node>())
	{
	}

	/// Constructor for Condition.
	/// @param firstPart - node we want to compare.
	/// @param comparator - comparator symbol node (==, !=).
	/// @param secondPart - node we want to compare.
	Condition(const QSharedPointer<Node> &firstPart
			, const QSharedPointer<Node> &comparator
			, const QSharedPointer<Node> &secondPart)
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
