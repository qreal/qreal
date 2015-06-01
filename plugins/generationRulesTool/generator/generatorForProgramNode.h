#pragma once

#include <QtCore/QSharedPointer>
#include <QtCore/QString>

#include <qrgui/plugins/interpretedPluginInterface/interpretedPluginInterface.h>

#include "ast/program.h"

#include "commonInfo/variablesTable.h"
#include "commonInfo/generatorConfigurer.h"

namespace generationRules {
namespace generator {

/// Class that generates result for program node.
class GeneratorForProgramNode
{
public:
	/// Returns generated string.
	/// @param programNode - node to generate code for.
	/// @param generatorConfigurer - information about model and metamodel.
	static QString generatedResult(QSharedPointer<simpleParser::ast::Program> programNode
			, GeneratorConfigurer generatorConfigurer);
};

}
}
