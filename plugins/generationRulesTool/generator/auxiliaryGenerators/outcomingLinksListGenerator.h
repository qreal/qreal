#pragma once

#include <QtCore/QString>
#include <QtCore/QSharedPointer>

#include <qrgui/plugins/interpretedPluginInterface/interpretedPluginInterface.h>

#include "ast/outcomingLinks.h"

namespace generationRules {
namespace generator {

class OutcomingLinksListGenerator
{
public:
	static qReal::IdList generatedList(QSharedPointer<simpleParser::ast::OutcomingLinks> linksIdentifierNode
			, qReal::LogicalModelAssistInterface *logicalModelInterface
			, const qReal::Id elementId
			);
};

}
}
