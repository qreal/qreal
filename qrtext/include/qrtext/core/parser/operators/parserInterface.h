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

#pragma once

#include <QtCore/QSet>

#include "qrtext/core/ast/node.h"
#include "qrtext/core/parser/tokenStream.h"
#include "qrtext/core/parser/parserContext.h"

namespace qrtext {
namespace core {

/// Interface for all elementary parsers.
template<typename TokenType>
class ParserInterface
{
public:
	virtual ~ParserInterface() {}

	/// Parses given token stream in given context and returns AST with result or nullptr if failed.
	virtual QSharedPointer<ast::Node> parse(TokenStream<TokenType> &tokenStream
			, ParserContext<TokenType> &parserContext) const = 0;

	/// Returns FIRST set for a parser.
	virtual QSet<TokenType> first() const = 0;
};

}
}
