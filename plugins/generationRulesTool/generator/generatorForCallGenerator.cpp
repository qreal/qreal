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

#include "generatorForCallGenerator.h"

#include "commonGenerator.h"
#include "generatorForGeneratorNode.h"

#include "ast/identifier.h"

#include "treeGeneratorFromString.h"
#include "auxiliaryGenerators/generatorForElementIdentifierNode.h"
#include "commonInfo/typeQualifier.h"

using namespace generationRules::generator;
using namespace simpleParser::ast;

QString GeneratorForCallGenerator::generatedResult(const QSharedPointer<CallGeneratorFor> &callGeneratorForNode
		, const GeneratorConfigurer &generatorConfigurer
		, ScopeInfo &scopeInfo)
{
	const auto calledIdentifier = qrtext::as<ElementIdentifier>(callGeneratorForNode->identifier());
	const auto generatorNameNode = qrtext::as<Identifier>(callGeneratorForNode->generatorName());

	return commonGeneratedString(calledIdentifier, generatorNameNode
			, generatorConfigurer, scopeInfo);
}

QString GeneratorForCallGenerator::commonGeneratedString(const QSharedPointer<ElementIdentifier> &calledIdentifier
		, const QSharedPointer<Identifier> &generatorNameNode
		, const GeneratorConfigurer &generatorConfigurer
		, ScopeInfo &scopeInfo)
{
	const auto currentElementId = GeneratorForElementIdentifierNode::neededElementId(calledIdentifier
			, generatorConfigurer, scopeInfo);
	const auto currentElementType = TypeQualifier::elementIdentifierType(calledIdentifier
			, generatorConfigurer, scopeInfo);

	const auto diagramId = generatorConfigurer.diagramId();
	const auto editorManagerInterface = generatorConfigurer.editorManagerInterface();

	const auto elementIdInMetamodel = idInMetamodel(editorManagerInterface, currentElementType, diagramId);

	const auto generationRuleForCurrentElement = editorManagerInterface->generationRule(elementIdInMetamodel);
	QSharedPointer<Node> generatedTree =
			TreeGeneratorFromString::generatedTreeFromString(generationRuleForCurrentElement);

	scopeInfo.currentScope().changeCurrentId(currentElementId);

	QString resultOfGeneration = "";
	if (!generatorNameNode) {
		resultOfGeneration = CommonGenerator::generatedResult(generatedTree, generatorConfigurer, scopeInfo);
	} else {
		QString generatorName = generatorNameNode->name();
		scopeInfo.currentScope().changeCurrentGeneratorName(generatorName);

		resultOfGeneration = CommonGenerator::generatedResult(generatedTree, generatorConfigurer, scopeInfo);
	}

	scopeInfo.currentScope().removeLastCurrentId();

	return resultOfGeneration;
}

qReal::Id GeneratorForCallGenerator::idInMetamodel(qReal::EditorManagerInterface *editorManagerInterface
		, const QString &elementName
		, const qReal::Id &diagramId)
{
	qReal::Id elementId;

	if (!editorManagerInterface->elementsWithTheSameName(diagramId, elementName, "MetaEntityNode").isEmpty()) {
		elementId = editorManagerInterface->elementsWithTheSameName(diagramId, elementName, "MetaEntityNode").first();
	} else {
		if (!editorManagerInterface->elementsWithTheSameName(diagramId, elementName, "MetaEntityEdge").isEmpty()) {
			elementId = editorManagerInterface->elementsWithTheSameName(diagramId, elementName
					, "MetaEntityEdge").first();
		} else {
			qDebug() << "Element " + elementName + " in metamodel for callGeneratorFor not found!";
			elementId = qReal::Id::rootId();
		}
	}

	return elementId;
}
