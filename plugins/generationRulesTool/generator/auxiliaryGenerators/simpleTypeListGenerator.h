#pragma once

#include <QtCore/QString>
#include <QtCore/QSharedPointer>

#include <qrgui/plugins/interpretedPluginInterface/interpretedPluginInterface.h>

#include "ast/identifier.h"

namespace generationRules {
namespace generator {

class SimpleTypeListGenerator
{
public:
	static qReal::IdList generatedList(QSharedPointer<simpleParser::ast::Identifier> linksIdentifierNode
			, qReal::LogicalModelAssistInterface *logicalModelInterface
			, const qReal::Id elementId
			);
};

}
}
