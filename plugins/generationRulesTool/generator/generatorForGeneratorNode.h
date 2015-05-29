#pragma once

#include <qrgui/plugins/interpretedPluginInterface/interpretedPluginInterface.h>

#include <QtCore/QSharedPointer>
#include <QtCore/QString>

#include "ast/generator.h"

#include "commonInfo/variablesTable.h"
#include "commonInfo/generatorConfigurer.h"

namespace generationRules {
namespace generator {

class GeneratorForGeneratorNode
{
public:
	static QString generatedResult(QSharedPointer<simpleParser::ast::Generator> generatorNode
			, GeneratorConfigurer generatorConfigurer);
};

}
}
