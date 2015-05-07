#pragma once

#include <QtCore/QString>
#include <QtCore/QSharedPointer>

#include <qrgui/plugins/interpretedPluginInterface/interpretedPluginInterface.h>

#include "ast/elementIdentifier.h"
#include "ast/identifier.h"
#include "ast/node.h"

#include "variablesTable.h"
#include "generatorConfigurer.h"

namespace generationRules {
namespace generator {

class GeneratorForElementIdentifierNode
{
public:
	static qReal::Id neededElementId(QSharedPointer<simpleParser::ast::ElementIdentifier> elementIdentifierNode
			, GeneratorConfigurer generatorCofigurer
			);
};

}
}
