#pragma once

#include <qrgui/plugins/interpretedPluginInterface/interpretedPluginInterface.h>

#include <QtCore/QSharedPointer>
#include <QtCore/QString>

#include "ast/generator.h"

#include "variablesTable.h"

namespace generationRules {
namespace generator {

class GeneratorForGeneratorNode
{
public:
	static QString generatedResult(QSharedPointer<simpleParser::ast::Generator> generatorNode
			, qReal::LogicalModelAssistInterface *logicalModelInterface
			, VariablesTable tableOfVariables
			, qReal::EditorManagerInterface *editorManagerInterface
			, const QString &wantedGeneratorName
			, const qReal::Id elementId = qReal::Id::rootId()
			, const QString &basicElementType = ""
			, const QString &basicElementName = ""
			);
};

}
}
