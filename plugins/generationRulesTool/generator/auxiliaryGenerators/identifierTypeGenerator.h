#pragma once

#include <QtCore/QString>
#include <QtCore/QSharedPointer>

#include "ast/list.h"

namespace generationRules {
namespace generator {

class IdentifierTypeGenerator
{
public:
	static QString variableType(QSharedPointer<simpleParser::ast::List> listNode);
};

}
}
