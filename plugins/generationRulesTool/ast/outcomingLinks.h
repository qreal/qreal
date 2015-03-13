#pragma once

#include "node.h"

namespace simpleParser {
namespace ast {

class OutcomingLinks : public Node
{
public:
	OutcomingLinks(QSharedPointer<Node> const &identifier)
		: mIdentifier(identifier)
	{
	}

	QSharedPointer<Node> const &identifier() const {
		return mIdentifier;
	}

private:
	QSharedPointer<Node> mIdentifier;
};

}
}
