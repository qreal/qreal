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

/// Class that represents generateToFile node.
class GenerateToFile : public Node
{
public:
	/// Constructor.
	/// @param fileName - name of file we want to generate code into.
	/// @param program - program part.
	GenerateToFile(const QSharedPointer<Node> &fileName
			, const QSharedPointer<Node> &program)
		: mFileName(fileName)
		, mProgram(program)
	{
	}

	/// Returns file name part.
	const QSharedPointer<Node> &fileName() const
	{
		return mFileName;
	}

	/// Returns program part.
	const QSharedPointer<Node> &program() const
	{
		return mProgram;
	}

private:
	QSharedPointer<Node> mFileName;
	QSharedPointer<Node> mProgram;
};

}
}
