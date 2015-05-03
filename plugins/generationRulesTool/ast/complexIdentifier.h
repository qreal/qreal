#pragma once

#include "node.h"

namespace simpleParser {
namespace ast {

/// Class that represents identifier which contains of two parts separated by dot.
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
	QSharedPointer<Node> identifierPart() const
	{
		return mIdentifierPart;
	}

	/// Returns second part of complex identifier.
	QSharedPointer<Node> propertyPart() const
	{
		return mPropertyPart;
	}

private:
	QSharedPointer<Node> mIdentifierPart;
	QSharedPointer<Node> mPropertyPart;
};
}
}
