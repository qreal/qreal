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
	/// This function is called if complex identifier name is the same as basic name in foreach.
	/// For example, foreach (x in State) {x.property}
	static QString generatedResultForNodeWithBasicName(
			const qReal::Id elementId
			, qReal::LogicalModelAssistInterface *logicalModelInterface
			, const QString &property);

	/// This function is called when we have SomeType.property, and element od SomeType type is unique.
	static QString generatedResultForNodeWithUniqueType(
			qReal::LogicalModelAssistInterface *logicalModelInterface
			, const QString &type
			, const QString &propertyName);

	/// This function is called for .transitionEnd
	static QString generatedResultForOutcomingLink(
			const qReal::Id linkId
			, qReal::LogicalModelAssistInterface *logicalModelInterface
			, const QString &propertyName);

	/// This function is called if the variable is in variables table, but is not the same as basic variable name in foreach.
	/// TODO: realize.
	static QString generatedResultForOtherVariable(
			const QString &variableName
			, qReal::LogicalModelAssistInterface *logicalModelInterface
			, const QString &propertyName
			);
};

}
}
