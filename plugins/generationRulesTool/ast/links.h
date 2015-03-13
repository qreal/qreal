#pragma once

#include "node.h"

namespace simpleParser {
namespace ast {

class Links : public Node
{
public:
	Links(QSharedPointer<Node> const &identifier)
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
