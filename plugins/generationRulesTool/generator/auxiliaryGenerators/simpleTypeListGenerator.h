#pragma once

#include <QtCore/QString>
#include <QtCore/QSharedPointer>

#include <qrgui/plugins/interpretedPluginInterface/interpretedPluginInterface.h>

#include "ast/identifier.h"

namespace generationRules {
namespace generator {

/// Class that generates list of ids for simple type list node.
class SimpleTypeListGenerator
{
public:
	/// Returns list of ids for given list node.
	/// @param simpleIdentifierNode - simple identifier node.
	/// @param logicalModelInterface - information about model.
	static qReal::IdList generatedList(const QSharedPointer<simpleParser::ast::Identifier> &simpleIdentifierNode
			, qReal::LogicalModelAssistInterface *logicalModelInterface
			);
};

}
}
