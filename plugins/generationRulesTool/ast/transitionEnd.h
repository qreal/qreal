#pragma once

#include "node.h"

namespace simpleParser {
namespace ast {

class TransitionEnd : public Node
{
public:
	explicit TransitionEnd(QSharedPointer<Node> const &firstIdentifier)
		: mFirstIdentifier(firstIdentifier)
	{
	}

	QSharedPointer<Node> const &firstIdentifier() const {
		return mFirstIdentifier;
	}

private:
	QSharedPointer<Node> mFirstIdentifier;
};

}
}
