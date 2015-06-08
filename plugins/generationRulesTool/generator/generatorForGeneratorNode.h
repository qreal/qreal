#pragma once

#include <qrgui/plugins/interpretedPluginInterface/interpretedPluginInterface.h>

#include <QtCore/QSharedPointer>
#include <QtCore/QString>

#include "ast/generator.h"

#include "commonInfo/variablesTable.h"
#include "commonInfo/generatorConfigurer.h"

namespace generationRules {
namespace generator {

/// Class that generates code for generator node.
class GeneratorForGeneratorNode
{
public:
	/// Returns generated string.
	/// @param generatorNode - node to generate code for.
	/// @param generatorConfigurer - information about model and metamodel.
	static QString generatedResult(const QSharedPointer<simpleParser::ast::Generator> &generatorNode
			, const GeneratorConfigurer &generatorConfigurer);
};

}
}
