#pragma once

#include "node.h"

namespace simpleParser {
namespace ast {

/// Class that represents links node.
class Links : public Node
{
public:
	/// Constructor.
	/// @param linkType - type of links (for example, SomeType in links(SomeType))
	explicit Links(const QSharedPointer<Node> &linkType)
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
