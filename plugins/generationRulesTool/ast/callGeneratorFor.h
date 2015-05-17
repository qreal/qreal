#pragma once

#include "node.h"

namespace simpleParser {
namespace ast {

/// Class that represents callGeneratorFor node.
class CallGeneratorFor : public Node
{
public:
	/// Constructor.
	/// @param identifier - identifier part (for example, element in callGeneratorFor(element, generatorName).
	/// @param generator - optional generator name (generatorName in previous example).
	CallGeneratorFor(QSharedPointer<Node> const &identifier
			, QSharedPointer<Node> const &generator = QSharedPointer<Node>())
		: mIdentifier(identifier)
		, mGeneratorName(generator)
	{
	}

	/// Returns identifier part.
	QSharedPointer<Node> const &identifier() const
	{
		return mIdentifier;
	}

	/// Returns generatorName part.
	QSharedPointer<Node> const &generatorName() const
	{
		return mGeneratorName;
	}

private:
	QSharedPointer<Node> mIdentifier;
	QSharedPointer<Node> mGeneratorName;
};

}
}

