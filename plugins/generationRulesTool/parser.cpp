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

#include "parser.h"

#include "ast/complexIdentifier.h"
#include "ast/foreach.h"
#include "ast/identifier.h"
#include "ast/newline.h"
#include "ast/node.h"
#include "ast/program.h"
#include "ast/text.h"

using namespace simpleParser;

using namespace qrtext::core;

simpleParser::Parser::Parser(QList<qrtext::core::Error> &errors)
	: qrtext::core::Parser<TokenTypes>(grammar(), errors)
{
}

QSharedPointer<qrtext::core::ParserInterface<TokenTypes>> simpleParser::Parser::grammar()
{
	ParserRef<TokenTypes> statement;

	auto program = *statement
			>> [] (QSharedPointer<TemporaryList> statementList) {
				QList<QSharedPointer<ast::Node>> result;
				for (auto const &stat : statementList->list()) {
					if (stat->is<TemporaryList>()) {
						// it is a list of assignments
						for (auto const &assignment : qrtext::as<TemporaryList>(stat)->list()) {
							result << assignment;
						}
					} else {
						// It is expression.
						result << stat;
					}
				}

				if (result.size() == 1) {
					// Do not create Program node for simple expression.
					return result.first();
				} else {
					return qrtext::wrap(new ast::Program(result));
				}
	};

	auto identifier = TokenTypes::identifier
			>> [] (Token<TokenTypes> const &token) {
				return new ast::Identifier(token.lexeme());
	};

	auto complexIdentifier = (identifier & -TokenTypes::dot & identifier)
			>> [] (QSharedPointer<TemporaryPair> tokens) {
				auto const identifier = tokens->left();
				auto const property = tokens->right();

				return qrtext::wrap(new ast::ComplexIdentifier(identifier, property));
	};

	auto foreachStatement = (-TokenTypes::foreachKeyword & -TokenTypes::openingBracket & TokenTypes::identifier
				& -TokenTypes::closingBracket & -TokenTypes::openingCurlyBracket & program & -TokenTypes::closingCurlyBracket)
			>> [] (QSharedPointer<TemporaryPair> statementPair) {
				auto const identifier = statementPair->right();
				auto const program = statementPair->left();

				return qrtext::wrap(new ast::Foreach(identifier, program));
	};

	auto text = TokenTypes::text
			>> [] (Token<TokenTypes> const &token) {
				return new ast::Text(token.lexeme());
	};

	auto newline = TokenTypes::newlineKeyword
			>> [] (Token<TokenTypes> const &token) {
				Q_UNUSED(token);
				return new ast::Newline();
	};

	statement = text | newline | complexIdentifier | foreachStatement;

	return program.parser();
}
