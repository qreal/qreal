#pragma once

#include <QtCore/QString>
#include <QtCore/QSharedPointer>

#include <qrgui/plugins/interpretedPluginInterface/interpretedPluginInterface.h>

#include "ast/node.h"

#include "commonInfo/variablesTable.h"
#include "commonInfo/generatorConfigurer.h"

namespace generationRules {
namespace generator {

/// Class that redirects node to appropriate generator.
class CommonGenerator
{
public:
	/// Returns generated string.
	/// @param node - node to generate code for.
	/// @param generatorConfigurer - information about model and metamodel.
	static QString generatedResult(const QSharedPointer<simpleParser::ast::Node> &node
			, const GeneratorConfigurer &generatorConfigurer
			);
};
}
}
