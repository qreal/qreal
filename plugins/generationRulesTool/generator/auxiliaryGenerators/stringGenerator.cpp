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

#include "stringGenerator.h"

#include "ast/complexIdentifier.h"
#include "ast/text.h"

#include "generator/generatorForComplexIdentifierNode.h"

using namespace generationRules::generator;
using namespace simpleParser::ast;

QString StringGenerator::generatedString(
		const QSharedPointer<String> &stringNode
		, const GeneratorConfigurer &generatorConfigurer
		, ScopeInfo &scopeInfo)
{
	const auto identifierPart = qrtext::as<ComplexIdentifier>(stringNode->identifierPart());
	const auto generatedString = GeneratorForComplexIdentifierNode::generatedResult(identifierPart, generatorConfigurer
			, scopeInfo).toString();

	const auto textPart = qrtext::as<Text>(stringNode->textPart());
	const auto text = textPart->text();

	return generatedString + text;
}
