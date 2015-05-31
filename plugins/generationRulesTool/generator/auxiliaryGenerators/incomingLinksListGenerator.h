#pragma once

#include <QtCore/QString>
#include <QtCore/QSharedPointer>

#include <qrgui/plugins/interpretedPluginInterface/interpretedPluginInterface.h>

#include "ast/identifier.h"

#include "generator/commonInfo/variablesTable.h"
#include "generator/commonInfo/currentScope.h"

namespace generationRules {
namespace generator {

/// Class that generates list of all incoming links of given type.
class IncomingLinksListGenerator
{
public:
	/// Returns list of links ids by type.
	/// @param linksIdentifierNode - first part (for example, identName in identName.incomingLinks(SpecificType)).
	/// @param linksTypeNode - second part (SpecificType in previous example).
	/// @param logicalModelInterface - information about model.
	/// @param variablesTable - table of variables.
	static qReal::IdList generatedList(QSharedPointer<simpleParser::ast::Node> linksIdentifierNode
			, QSharedPointer<simpleParser::ast::Identifier> linksTypeNode
			, qReal::LogicalModelAssistInterface *logicalModelInterface
			, VariablesTable variablesTable
			, CurrentScope currentScope
			);
};
}
}
