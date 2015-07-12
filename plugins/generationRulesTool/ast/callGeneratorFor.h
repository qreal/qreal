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

/// Class that represents callGeneratorFor node.
class CallGeneratorFor : public Node
{
public:
	/// Constructor.
	/// @param identifier - identifier part (for example, element in callGeneratorFor(element, generatorName)).
	/// @param generator - optional generator name (generatorName in previous example).
	explicit CallGeneratorFor(const QSharedPointer<Node> &identifier
			, const QSharedPointer<Node> &generator = QSharedPointer<Node>())
		: mIdentifier(identifier)
		, mGeneratorName(generator)
	{
	}

	/// Returns identifier part.
	const QSharedPointer<Node> &identifier() const
	{
		return mIdentifier;
	}

	/// Returns generatorName part.
	const QSharedPointer<Node> &generatorName() const
	{
		return mGeneratorName;
	}

private:
	QSharedPointer<Node> mIdentifier;
	QSharedPointer<Node> mGeneratorName;
};

}
}

