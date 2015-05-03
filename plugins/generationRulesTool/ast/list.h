#pragma once

#include "node.h"

namespace simpleParser {
namespace ast {

/// Class that represents list node;
/// List can represent simple type or list of links.
class List : public Node
{
public:
	List(QSharedPointer<Node> identifier
			, QSharedPointer<Node> optionalLinkPart = QSharedPointer<Node>())
		: mIdentifier(identifier)
		, mOptionalLinkPart(optionalLinkPart)
	{
	}

	/// Returns identifier part of list identifier.
	QSharedPointer<Node> identifier() const {
		return mIdentifier;
	}

	/// Returns link part of list identifier.
	QSharedPointer<Node> optionalLinkPart() const {
		return mOptionalLinkPart;
	}

private:
	QSharedPointer<Node> mIdentifier;
	QSharedPointer<Node> mOptionalLinkPart;
};

}
}
