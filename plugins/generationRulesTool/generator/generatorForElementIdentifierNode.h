#pragma once

#include <QtCore/QString>
#include <QtCore/QSharedPointer>

#include <qrgui/plugins/interpretedPluginInterface/interpretedPluginInterface.h>

#include "ast/elementIdentifier.h"
#include "ast/identifier.h"
#include "ast/node.h"

#include "variablesTable.h"

namespace generationRules {
namespace generator {

class GeneratorForElementIdentifierNode
{
public:
	static qReal::IdList neededElementId(QSharedPointer<simpleParser::ast::ElementIdentifier> elementIdentifierNode
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
