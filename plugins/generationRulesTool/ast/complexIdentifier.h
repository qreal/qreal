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

/// Class that represents identifier which consists of two parts separated by dot.
class ComplexIdentifier : public Node
{
public:
	/// Constructor for ComplexIdentifier.
	/// @param identifierPart - part before dot.
	/// @param propertyPart - part after dot (is optional).
	ComplexIdentifier(const QSharedPointer<Node> &identifierPart
			, const QSharedPointer<Node> &propertyPart)
		: mIdentifierPart(identifierPart)
		, mPropertyPart(propertyPart)
	{
	}

	/// Returns first part of complex identifier.
	const QSharedPointer<Node> &identifierPart() const
	{
		return mIdentifierPart;
	}

	/// Returns second part of complex identifier.
	const QSharedPointer<Node> &propertyPart() const
	{
		return mPropertyPart;
	}

private:
	QSharedPointer<Node> mIdentifierPart;
	QSharedPointer<Node> mPropertyPart;
};
}
}
