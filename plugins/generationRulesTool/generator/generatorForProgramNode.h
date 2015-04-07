#pragma once

#include <QtCore/QSharedPointer>
#include <QtCore/QString>

#include <qrgui/plugins/interpretedPluginInterface/interpretedPluginInterface.h>

#include "ast/program.h"

#include "variablesTable.h"

namespace generationRules {
namespace generator {

class GeneratorForProgramNode
{
public:
	static QString generatedResult(QSharedPointer<simpleParser::ast::Program> programNode
			, qReal::LogicalModelAssistInterface *logicalModelInterface
			, VariablesTable tableOfVariables
			, qReal::EditorManagerInterface *editorManagerInterface
			, qReal::Id const elementId
			, const QString &elementType = ""
			, const QString &elementName = ""
			);
};

}
}
