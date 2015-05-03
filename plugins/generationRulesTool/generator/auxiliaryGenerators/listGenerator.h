#pragma once

#include <QtCore/QString>
#include <QtCore/QSharedPointer>

#include <qrgui/plugins/interpretedPluginInterface/interpretedPluginInterface.h>

#include "ast/list.h"

namespace generationRules {
namespace generator {

class ListGenerator
{
public:
	static qReal::IdList listOfIds(QSharedPointer<simpleParser::ast::List> listNode
			, qReal::LogicalModelAssistInterface *logicalModelInterface
			, const qReal::Id elementId);
};
}
}
