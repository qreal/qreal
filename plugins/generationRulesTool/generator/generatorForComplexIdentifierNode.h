#pragma once

#include <QtCore/QString>
#include <QtCore/QSharedPointer>

#include <qrgui/plugins/interpretedPluginInterface/interpretedPluginInterface.h>

#include "ast/complexIdentifier.h"
#include "ast/identifier.h"
#include "ast/node.h"

#include "variablesTable.h"

namespace generationRules {
namespace generator {

class GeneratorForComplexIdentifierNode
{
public:
	///
	/// @param elementType - type of basic element in foreach
	/// @param elementName - name of basic element in foreach
	static QString generatedResult(QSharedPointer<simpleParser::ast::ComplexIdentifier> complexIdentifierNode
			, qrRepo::LogicalRepoApi *metamodelRepoApi
			, qrRepo::RepoControlInterface *modelRepo
			, qReal::LogicalModelAssistInterface *logicalModelInterface
			, VariablesTable tableOfVariables
			, qReal::Id const elementId
			, const QString &elementType = ""
			, const QString &elementName = ""
			);

private:
	static QString generatedResultForCorrectNode(QSharedPointer<simpleParser::ast::Identifier> typeNode
			, QSharedPointer<simpleParser::ast::Node> secondNode
			, qReal::LogicalModelAssistInterface *logicalModelInterface
			, const qReal::Id elementId);
};

}
}
