#pragma once

#include <QtCore/QString>
#include <QtCore/QSharedPointer>

#include <qrgui/plugins/interpretedPluginInterface/interpretedPluginInterface.h>
#include <qrgui/plugins/pluginManager/editorManagerInterface.h>

#include "ast/callGeneratorFor.h"
#include "ast/elementIdentifier.h"
#include "ast/identifier.h"

#include "commonInfo/variablesTable.h"
#include "commonInfo/generatorConfigurer.h"

namespace generationRules {
namespace generator {

class GeneratorForCallGenerator
{
public:
	static QString generatedResult(QSharedPointer<simpleParser::ast::CallGeneratorFor> callGeneratorForNode
			, GeneratorConfigurer generatorConfigurer
			);

	static QString commonGeneratedString(QSharedPointer<simpleParser::ast::ElementIdentifier> calledIdentifier
			, QSharedPointer<simpleParser::ast::Identifier> generatorNameNode
			, GeneratorConfigurer generatorConfigurer);

private:
	static qReal::Id idInMetamodel(qReal::EditorManagerInterface *editorManagerInterface
			, const QString &elementName
			, const qReal::Id &diagramId);
};

}
}
