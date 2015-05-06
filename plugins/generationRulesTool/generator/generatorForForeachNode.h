#pragma once

#include <qrgui/plugins/interpretedPluginInterface/interpretedPluginInterface.h>

#include <QtCore/QSharedPointer>
#include <QtCore/QString>

#include "ast/foreach.h"

#include "variablesTable.h"
#include "generatorConfigurer.h"

namespace generationRules {
namespace generator {

class GeneratorForForeachNode
{
public:
	static QString generatedResult(QSharedPointer<simpleParser::ast::Foreach> foreachNode
			, GeneratorConfigurer generatorConfigurer
			, const QString &generatorName = ""
			);
};

}
}
