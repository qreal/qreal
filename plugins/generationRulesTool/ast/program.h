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

#include "qrtext/core/ast/node.h"
#include "node.h"

namespace simpleParser {
namespace ast {

/// Class that represents program node.
class Program : public Node
{
public:
	/// Constructor.
	/// @param statements - a list of statements in a program.
	explicit Program(const QList<QSharedPointer<Node>> &statements)
		: mStatements(statements)
	{
	}

	QList<QSharedPointer<Node>> children() const override
	{
		return mStatements;
	}

private:
	QList<QSharedPointer<Node>> mStatements;
};

}
}
