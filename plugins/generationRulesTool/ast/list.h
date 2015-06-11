#pragma once

#include "node.h"

namespace simpleParser {
namespace ast {

/// Class that represents list node;
/// List can represent simple type or list of links.
class List : public Node
{
public:
	/// Constructor.
	/// @param identifier - identifier part (for example, element in element.outcomingLinks).
	/// @param optionalLinkPart - link part (outcomingLinks in previous example).
	List(const QSharedPointer<Node> &identifier
			, const QSharedPointer<Node> &optionalLinkPart = QSharedPointer<Node>())
		: mIdentifier(identifier)
		, mOptionalLinkPart(optionalLinkPart)
	{
	}

	/// Returns identifier part of list identifier.
	const QSharedPointer<Node> &identifier() const
	{
		return mIdentifier;
	}

	/// Returns link part of list identifier.
	const QSharedPointer<Node> &optionalLinkPart() const
	{
		return mOptionalLinkPart;
	}

private:
	QSharedPointer<Node> mIdentifier;
	QSharedPointer<Node> mOptionalLinkPart;
};

}
}
