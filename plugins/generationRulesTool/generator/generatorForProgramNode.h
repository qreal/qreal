#pragma once

#include <QtCore/QSharedPointer>
#include <QtCore/QString>

#include <qrgui/plugins/interpretedPluginInterface/interpretedPluginInterface.h>

#include "ast/program.h"

#include "variablesTable.h"
#include "generatorConfigurer.h"

namespace generationRules {
namespace generator {

class GeneratorForProgramNode
{
public:
	static QString generatedResult(QSharedPointer<simpleParser::ast::Program> programNode
			, GeneratorConfigurer generatorConfigurer
			, const QString &generatorName
			);
};

}
}
