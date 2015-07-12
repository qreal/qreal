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

#include "linksListGenerator.h"

using namespace generationRules::generator;
using namespace simpleParser::ast;

qReal::IdList LinksListGenerator::generatedList(
		const QSharedPointer<Node> &linksIdentifierNode
		, const QSharedPointer<Identifier> &linksTypeNode
		, qReal::LogicalModelAssistInterface *logicalModelInterface
		, VariablesTable &variablesTable
		, const CurrentScope &currentScope)
{
	const auto neededElementId = elementId(linksIdentifierNode, variablesTable, currentScope);
	const auto allLinks = logicalModelInterface->logicalRepoApi().links(neededElementId);
	return linksOfNeededType(linksTypeNode, allLinks);
}

qReal::Id LinksListGenerator::elementId(const QSharedPointer<Node> &linksIdentifierNode
		, VariablesTable &variablesTable
		, const CurrentScope &currentScope)
{
	qReal::Id elementId;
	if (linksIdentifierNode->is<Identifier>()) {
		const auto elementName = qrtext::as<Identifier>(linksIdentifierNode)->name();
		elementId = variablesTable.currentId(elementName);
	} else {
		elementId = currentScope.currentId();
	}

	return elementId;
}

qReal::IdList LinksListGenerator::linksOfNeededType(const QSharedPointer<Identifier> &linksTypeNode
		, const qReal::IdList &allLinks)
{
	qReal::IdList linksList;

	const auto linksType = linksTypeNode->name();

	for (const auto &currentLink : allLinks) {
		if (currentLink.element() == linksType) {
			linksList << currentLink;
		}
	}

	return linksList;
}
