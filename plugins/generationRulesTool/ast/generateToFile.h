#pragma once

#include "node.h"

namespace simpleParser {
namespace ast {

class GenerateToFile : public Node
{
public:
	GenerateToFile(QSharedPointer<Node> const &identifier
			, QSharedPointer<Node> const &fileName
			, QSharedPointer<Node> const &generator = QSharedPointer<Node>())
		: mIdentifier(identifier)
		, mFileName(fileName)
		, mGeneratorName(generator)
	{
	}

	QSharedPointer<Node> const &identifier() const
	{
		return mIdentifier;
	}

	QSharedPointer<Node> const &fileName() const
	{
		return mFileName;
	}

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
