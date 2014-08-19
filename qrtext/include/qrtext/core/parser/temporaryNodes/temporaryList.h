#pragma once

#include <QtCore/QList>

#include "qrtext/core/ast/node.h"

namespace qrtext {
namespace core {
namespace parser {

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
}
