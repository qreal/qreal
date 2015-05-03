#pragma once

#include "node.h"

namespace simpleParser {
namespace ast {

class Links : public Node
{
public:
	explicit Links(QSharedPointer<Node> linkType)
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
