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

#include "qrtext/core/parser/parserRef.h"
#include "qrtext/core/parser/operators/parserInterface.h"
#include "qrtext/core/parser/temporaryNodes/temporaryDiscardableNode.h"

namespace qrtext {
namespace core {

/// Elementary parser for parsing optional parts in form of A = B?. Returns result of a parser if optional part is
/// present or TemporaryDiscardableNode if not.
template<typename TokenType>
class OptionalParser : public ParserInterface<TokenType>
{
public:
	/// Constructor. Takes parser for optional part.
	OptionalParser(const ParserRef<TokenType> &parser)
		: mParser(parser)
	{
	}

	QSharedPointer<ast::Node> parse(TokenStream<TokenType> &tokenStream
			, ParserContext<TokenType> &parserContext) const override
	{
		if (mParser->first().contains(tokenStream.next().token())) {
			return mParser->parse(tokenStream, parserContext);
		}

		return wrap(new TemporaryDiscardableNode());
	}

	QSet<TokenType> first() const override
	{
		return mParser->first();
	}

private:
	ParserRef<TokenType> mParser;
};

}
}
