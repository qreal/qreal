#pragma once

#include "node.h"

namespace simpleParser {
namespace ast {

/// Class that represents generateToFile node.
class GenerateToFile : public Node
{
public:
	/// Constructor.
	/// @param identifier - identifier part.
	/// @param fileName - name of file we want to generate code into.
	/// @param generator - optional generatorName node.
	GenerateToFile(QSharedPointer<Node> const &identifier
			, QSharedPointer<Node> const &fileName
			, QSharedPointer<Node> const &generator = QSharedPointer<Node>())
		: mIdentifier(identifier)
		, mFileName(fileName)
		, mGeneratorName(generator)
	{
	}

	/// Returns identifier part.
	QSharedPointer<Node> const &identifier() const
	{
		return mIdentifier;
	}

	/// Returns file name part.
	QSharedPointer<Node> const &fileName() const
	{
		return mFileName;
	}

	/// Returns generator name part.
	QSharedPointer<Node> const &generatorName() const
	{
		return mGeneratorName;
	}

private:
	QSharedPointer<Node> mIdentifier;
	QSharedPointer<Node> mFileName;
	QSharedPointer<Node> mGeneratorName;
};

}
}
