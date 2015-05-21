#pragma once

#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtCore/QSharedPointer>

#include <qrgui/plugins/interpretedPluginInterface/interpretedPluginInterface.h>

#include "ast/complexIdentifier.h"
#include "ast/identifier.h"
#include "ast/node.h"

#include "variablesTable.h"
#include "generatorConfigurer.h"

namespace generationRules {
namespace generator {

class GeneratorForComplexIdentifierNode
{
public:
	/// @param complexIdentifier - identifier node.
	/// @param generatorConfigurer - all information about generator.
	static QVariant generatedResult(QSharedPointer<simpleParser::ast::ComplexIdentifier> complexIdentifierNode
			, GeneratorConfigurer generatorConfigurer);
};

}
}
