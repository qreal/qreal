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

/// Class that represents element identifier node
/// in the following forms: identifier, identifier.transitionEnd, identifier.transitionStart
class ElementIdentifier : public Node
{
public:
	/// Constructor for ElementIdentifier.
	/// @param identifierPart - part defore dot.
	/// @param optionalTransitionPart - optional part after dot.
	explicit ElementIdentifier(const QSharedPointer<Node> &identifierPart
			, const QSharedPointer<Node> &optionalTransitionPart = QSharedPointer<Node>())
		: mIdentifierPart(identifierPart)
		, mOptionalTransitionPart(optionalTransitionPart)
	{
	}

	/// Returns identifier part.
	const QSharedPointer<Node> &identifierPart() const
	{
		return mIdentifierPart;
	}

	/// Returns transition part.
	const QSharedPointer<Node> &optionalTransitionPart() const
	{
		return mOptionalTransitionPart;
	}

private:
	QSharedPointer<Node> mIdentifierPart;
	QSharedPointer<Node> mOptionalTransitionPart;
};

}
}
