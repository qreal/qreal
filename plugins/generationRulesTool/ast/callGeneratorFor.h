#pragma once

#include "node.h"

namespace simpleParser {
namespace ast {

class CallGeneratorFor : public Node
{
public:
	CallGeneratorFor(QSharedPointer<Node> const &identifier
			, QSharedPointer<Node> const &generator = QSharedPointer<Node>())
		: mIdentifier(identifier)
		, mGeneratorName(generator)
	{
	}

	QSharedPointer<Node> const &identifier() const {
		return mIdentifier;
	}

	QSharedPointer<Node> const &generatorName() const {
		return mGeneratorName;
	}

private:
	QSharedPointer<Node> mIdentifier;
	QSharedPointer<Node> mGeneratorName;
};

}
}

