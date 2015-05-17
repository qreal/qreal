#pragma once

#include "node.h"

namespace simpleParser {
namespace ast {

/// Class that represents element identifier node
/// in the following forms: identifier, identifier.transitionEnd, identifier.transitionStart
class ElementIdentifier : public Node
{
public:
	/// Constructor for ElementIdentifier.
	/// @param identifierPart - part defore dot.
	/// @param optionalTransitionPart - optional part after dot.
	ElementIdentifier(const QSharedPointer<Node> &identifierPart
			, const QSharedPointer<Node> &optionalTransitionPart = QSharedPointer<Node>())
		: mIdentifierPart(identifierPart)
		, mOptionalTransitionPart(optionalTransitionPart)
	{
	}

	/// Returns identifier part.
	const QSharedPointer<Node> &identifierPart() const
	{
		return mIdentifierPart;
	}

	/// Returns transition part.
	const QSharedPointer<Node> &optionalTransitionPart() const
	{
		return mOptionalTransitionPart;
	}

private:
	QSharedPointer<Node> mIdentifierPart;
	QSharedPointer<Node> mOptionalTransitionPart;
};

}
}
