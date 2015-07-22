/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "generatorForForeachNode.h"
#include "generatorForProgramNode.h"
#include "generatorForCallGenerator.h"

#include "ast/identifier.h"
#include "ast/outcomingLinks.h"
#include "ast/program.h"
#include "ast/callGeneratorFor.h"
#include "ast/list.h"
#include "ast/text.h"

#include "plugins/pluginManager/editorManagerInterface.h"

#include "auxiliaryGenerators/listGenerator.h"
#include "auxiliaryGenerators/identifierTypeGenerator.h"

using namespace generationRules::generator;
using namespace simpleParser::ast;

QString GeneratorForForeachNode::generatedResult(const QSharedPointer<Foreach> &foreachNode
		, const GeneratorConfigurer &generatorConfigurer
		, ScopeInfo &scopeInfo)
{
	const auto identifierPart = qrtext::as<Identifier>(foreachNode->identifier());
	const auto identifierName = identifierPart->name();

	const auto listPart = qrtext::as<List>(foreachNode->listPart());
	const auto logicalModelInterface = generatorConfigurer.logicalModelInterface();

	const auto identifierType = IdentifierTypeGenerator::variableType(listPart);

	qReal::IdList listOfElements = ListGenerator::listOfIds(listPart, logicalModelInterface
			, scopeInfo.variablesTable(), scopeInfo.currentScope());

	QString result;
	scopeInfo.variablesTable().addNewVariable(identifierName, identifierType, listOfElements);

	const auto actionNode = foreachNode->program();

	while (scopeInfo.variablesTable().nextIdExists(identifierName)) {
		result += resultForOneIteration(actionNode, generatorConfigurer, scopeInfo);
		scopeInfo.variablesTable().movePointer(identifierName);
	}

	const auto excludedText = qrtext::as<Text>(foreachNode->excludedText());
	if (excludedText) {
		scopeInfo.setExcludedText(excludedText->text());
	}

	scopeInfo.variablesTable().movePointer(identifierName);
	result += resultForOneIteration(actionNode, generatorConfigurer, scopeInfo);
	scopeInfo.setExcludedText(QString());

	scopeInfo.variablesTable().removeVariable(identifierName);

	return result;
}

QString GeneratorForForeachNode::resultForOneIteration(const QSharedPointer<Node> &actionNode
		, const GeneratorConfigurer &generatorConfigurer
		, ScopeInfo &scopeInfo)
{
	QString result = "";
	if (actionNode->is<Program>()) {
		result = GeneratorForProgramNode::generatedResult(qrtext::as<Program>(actionNode)
				, generatorConfigurer, scopeInfo);
	} else {
		if (actionNode->is<CallGeneratorFor>()) {
			result = GeneratorForCallGenerator::generatedResult(qrtext::as<CallGeneratorFor>(actionNode)
				, generatorConfigurer, scopeInfo);
		}
	}

	return result;
}
