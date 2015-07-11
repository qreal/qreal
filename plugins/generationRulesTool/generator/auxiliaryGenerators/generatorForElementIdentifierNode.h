#pragma once

#include <QtCore/QString>
#include <QtCore/QSharedPointer>

#include <qrgui/plugins/interpretedPluginInterface/interpretedPluginInterface.h>

#include "ast/elementIdentifier.h"
#include "ast/identifier.h"
#include "ast/node.h"

#include "generator/commonInfo/variablesTable.h"
#include "generator/commonInfo/generatorConfigurer.h"
#include "generator/commonInfo/scopeInfo.h"

namespace generationRules {
namespace generator {

/// Class that generates result for elementIdentifier node.
class GeneratorForElementIdentifierNode
{
public:
	/// Returns id of element identifier.
	/// @param elementIdentifierNode - element identifier node.
	/// @param generatorConfigurer - information about model and metamodel.
	/// @param scopeInfo - information about current scope.
	static qReal::Id neededElementId(const QSharedPointer<simpleParser::ast::ElementIdentifier> &elementIdentifierNode
			, const GeneratorConfigurer &generatorConfigurer
			, ScopeInfo &scopeInfo
			);
};

}
}
