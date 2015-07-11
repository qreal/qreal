#pragma once

#include <QtCore/QString>
#include <QtCore/QSharedPointer>

#include "ast/list.h"

namespace generationRules {
namespace generator {

/// Class that generates type of given list element.
class IdentifierTypeGenerator
{
public:
	/// Returns element type of given list.
	/// @param listNode - list node.
	static QString variableType(const QSharedPointer<simpleParser::ast::List> &listNode);
};

}
}
