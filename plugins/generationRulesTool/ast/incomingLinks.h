#pragma once

#include "node.h"

namespace simpleParser {
namespace ast {

class IncomingLinks : public Node
{
public:
	IncomingLinks(QSharedPointer<Node> const &identifier)
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
