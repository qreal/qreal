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

