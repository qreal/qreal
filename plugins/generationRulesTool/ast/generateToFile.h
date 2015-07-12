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
