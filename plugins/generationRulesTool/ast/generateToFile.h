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
	GenerateToFile(QSharedPointer<Node> const &fileName
			, QSharedPointer<Node> const &program)
		: mFileName(fileName)
		, mProgram(program)
	{
	}

	/// Returns file name part.
	QSharedPointer<Node> const &fileName() const
	{
		return mFileName;
	}

	/// Returns program part.
	QSharedPointer<Node> const &program() const
	{
		return mProgram;
	}

private:
	QSharedPointer<Node> mFileName;
	QSharedPointer<Node> mProgram;
};

}
}
