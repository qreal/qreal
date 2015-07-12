/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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
