#pragma once

#include <QtCore/QString>
#include <QtCore/QSharedPointer>

#include <qrgui/plugins/interpretedPluginInterface/interpretedPluginInterface.h>
#include <qrgui/plugins/pluginManager/editorManagerInterface.h>

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
			);

private:
	static qReal::Id idInMetamodel(qReal::EditorManagerInterface *editorManagerInterface
			, const QString &elementName
			, const qReal::Id &diagramId);
};

}
}
