#pragma once

#include <QtCore/QString>
#include <QtCore/QSharedPointer>

#include <qrgui/plugins/interpretedPluginInterface/interpretedPluginInterface.h>

#include "ast/list.h"

#include "generator/commonInfo/variablesTable.h"
#include "generator/commonInfo/currentScope.h"

namespace generationRules {
namespace generator {

/// Class that generates list of ids for list node.
class ListGenerator
{
public:
	/// Returns list of ids for given list node.
	/// @param listNode - list node.
	/// @param logicalModelInterface - model interface.
	/// @param variablesTable - table of variables.
	/// @param currentScope - information about current scope.
	static qReal::IdList listOfIds(const QSharedPointer<simpleParser::ast::List> &listNode
			, qReal::LogicalModelAssistInterface *logicalModelInterface
			, VariablesTable &variablesTable
			, const CurrentScope &currentScope);
};
}
}
