#pragma once

#include "node.h"

namespace simpleParser {
namespace ast {

class CallGeneratorFor : public Node
{
public:
	CallGeneratorFor(QSharedPointer<Node> const &identifier)
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

