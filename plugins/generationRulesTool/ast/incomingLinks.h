#pragma once

#include "node.h"

namespace simpleParser {
namespace ast {

/// Class that represents incomingLinks node.
class IncomingLinks : public Node
{
public:
	/// Constructor.
	/// @param linkType - type of incoming links (for example, SomeType in incomingLinks(SomeType))
	explicit IncomingLinks(QSharedPointer<Node> linkType)
		: mLinkType(linkType)
	{
	}

	/// Returns link type node.
	QSharedPointer<Node> linkType() const
	{
		return mLinkType;
	}

private:
	QSharedPointer<Node> mLinkType;
};

}
}
