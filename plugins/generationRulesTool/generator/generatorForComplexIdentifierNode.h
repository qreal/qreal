#pragma once

#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtCore/QSharedPointer>

#include <qrgui/plugins/interpretedPluginInterface/interpretedPluginInterface.h>

#include "ast/complexIdentifier.h"
#include "ast/identifier.h"
#include "ast/node.h"

#include "commonInfo/variablesTable.h"
#include "commonInfo/generatorConfigurer.h"
#include "commonInfo/scopeInfo.h"

namespace generationRules {
namespace generator {

/// Class that generates string for complexIdentifier node.
class GeneratorForComplexIdentifierNode
{
public:
	/// @param complexIdentifier - identifier node.
	/// @param generatorConfigurer - all information about generator.
	/// @param scopeInfo - information about current scope.
	static QVariant generatedResult(const QSharedPointer<simpleParser::ast::ComplexIdentifier> &complexIdentifierNode
			, const GeneratorConfigurer &generatorConfigurer
			, ScopeInfo &scopeInfo);
};

}
}
