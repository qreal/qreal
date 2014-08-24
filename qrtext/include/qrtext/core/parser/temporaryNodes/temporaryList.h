#pragma once

#include <QtCore/QList>

#include "qrtext/core/ast/node.h"

namespace qrtext {
namespace core {

/// Node that contains multiple other nodes and is used as temporary container (with connection). Shall be discarded
/// from resulting tree.
class TemporaryList : public ast::Node {
public:
	QList<QSharedPointer<ast::Node>> &list()
	{
		return mList;
	}

private:
	QList<QSharedPointer<ast::Node>> mList;
};

}
}
