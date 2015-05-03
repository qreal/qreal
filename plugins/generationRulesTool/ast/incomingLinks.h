#pragma once

#include "node.h"

namespace simpleParser {
namespace ast {

class IncomingLinks : public Node
{
public:
	explicit IncomingLinks(QSharedPointer<Node> linkType)
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
