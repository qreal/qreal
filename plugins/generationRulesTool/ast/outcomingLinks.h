#pragma once

#include "node.h"

namespace simpleParser {
namespace ast {

class OutcomingLinks : public Node
{
public:
	explicit OutcomingLinks(QSharedPointer<Node> linkType)
		: mLinkType(linkType)
	{
	}

	QSharedPointer<Node> linkType() const {
		return mLinkType;
	}

private:
	QSharedPointer<Node> mLinkType;
};
}
}
