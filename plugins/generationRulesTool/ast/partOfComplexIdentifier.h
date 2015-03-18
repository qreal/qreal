#pragma once

#include "node.h"

namespace simpleParser {
namespace ast {

class PartOfComplexIdentifier : public Node
{
public:
	explicit PartOfComplexIdentifier(QSharedPointer<Node> const &identifier)
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
