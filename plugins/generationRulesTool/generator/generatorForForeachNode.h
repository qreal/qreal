#pragma once

#include <qrgui/plugins/interpretedPluginInterface/interpretedPluginInterface.h>

#include <QtCore/QSharedPointer>
#include <QtCore/QString>

#include "ast/foreach.h"

#include "commonInfo/variablesTable.h"
#include "commonInfo/generatorConfigurer.h"
#include "commonInfo/scopeInfo.h"

namespace generationRules {
namespace generator {

/// Class that generates string for foreach node.
class GeneratorForForeachNode
{
public:
	/// Returns generated string.
	/// @param foreachNode - node to generate code for.
	/// @param generatorConfigurer - information about model and metamodel.
	/// @param scopeInfo - information about current scope.
	static QString generatedResult(const QSharedPointer<simpleParser::ast::Foreach> &foreachNode
			, const GeneratorConfigurer &generatorConfigurer
			, ScopeInfo &scopeInfo
			);

private:
	static QString resultForOneIteration(const QSharedPointer<simpleParser::ast::Node> &actionNode
			, const GeneratorConfigurer &generatorConfigurer
			, ScopeInfo &scopeInfo);
};

}
}
