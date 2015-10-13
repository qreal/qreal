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

/// Class that represents foreach node.
class Foreach : public Node
{
public:
	/// Constructor.
	/// @param identifier - iterator identifier.
	/// @param list - list to iterate.
	/// @param program - subprogram.
	Foreach(const QSharedPointer<Node> &identifier
			, const QSharedPointer<Node> &list
			, const QSharedPointer<Node> &program
			, const QSharedPointer<Node> &excludedText = QSharedPointer<Node>())
		: mIdentifier(identifier)
		, mList(list)
		, mProgram(program)
		, mExcludedText(excludedText)
	{
	}

	/// Returns identifier node.
	const QSharedPointer<Node> &identifier() const
	{
		return mIdentifier;
	}

	/// Returns list node.
	const QSharedPointer<Node> &listPart() const
	{
		return mList;
	}

	/// Returns program node.
	const QSharedPointer<Node> &program() const
	{
		return mProgram;
	}

	/// Returns excluded text.
	const QSharedPointer<Node> &excludedText() const
	{
		return mExcludedText;
	}

	QList<QSharedPointer<Node>> children() const override
	{
		return {mIdentifier, mList, mProgram};
	}

private:
	QSharedPointer<Node> mIdentifier;
	QSharedPointer<Node> mList;
	QSharedPointer<Node> mProgram;
	QSharedPointer<Node> mExcludedText;
};
}
}
