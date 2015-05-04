#pragma once

#include <QtCore/QString>
#include <QtCore/QSharedPointer>

#include <qrgui/plugins/interpretedPluginInterface/interpretedPluginInterface.h>

#include "ast/callGeneratorFor.h"

#include "variablesTable.h"
#include "generatorConfigurer.h"

namespace generationRules {
namespace generator {

class GeneratorForCallGenerator
{
public:
	static QString generatedResult(QSharedPointer<simpleParser::ast::CallGeneratorFor> callGeneratorForNode
			, GeneratorConfigurer generatorConfigurer
			, const QString &generatorName
			, qReal::Id const elementId
			, const QString &elementType = ""
			, const QString &elementName = ""
			);
};

}
}
