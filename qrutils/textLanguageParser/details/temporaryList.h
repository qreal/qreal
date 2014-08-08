#pragma once

#include <QtCore/QList>

#include "textLanguageParser/ast/node.h"

namespace textLanguageParser {
namespace details {

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
