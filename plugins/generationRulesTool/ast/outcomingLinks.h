#pragma once

#include "node.h"

namespace simpleParser {
namespace ast {

/// Class that represents outcomingLinks node.
class OutcomingLinks : public Node
{
public:
	/// Constructor.
	/// @param linkType - type of outcoming links.
	explicit OutcomingLinks(const QSharedPointer<Node> &linkType)
		: mLinkType(linkType)
	{
	}

	/// Returns link type node.
	const QSharedPointer<Node> &linkType() const
	{
		return mLinkType;
	}

private:
	QSharedPointer<Node> mLinkType;
};
}
}
