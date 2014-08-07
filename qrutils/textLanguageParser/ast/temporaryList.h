#pragma once

#include <QtCore/QList>

#include "node.h"

namespace textLanguageParser {
namespace ast {

/// @todo Move it somewhere, it is not a part of AST.
class TemporaryList : public Node {
public:
	QList<QSharedPointer<Node>> &list()
	{
		return mList;
	}

private:
	QList<QSharedPointer<Node>> mList;
};

}
}
