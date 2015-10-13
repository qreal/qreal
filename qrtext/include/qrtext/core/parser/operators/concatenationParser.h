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
#include "qrtext/core/parser/temporaryNodes/temporaryPair.h"
#include "qrtext/core/parser/temporaryNodes/temporaryDiscardableNode.h"
#include "qrtext/core/parser/temporaryNodes/temporaryErrorNode.h"

namespace qrtext {
namespace core {

/// Elementary parser for parsing concatenations in form of A = B C. If one of parsers returns TemporaryDiscardableNode,
/// it will return result of the other parser (connection of discardable will be lost), if both are discardable,
/// it will return discardable with corrected connection, if both are not discardable, it will return TemporaryPair.
template<typename TokenType>
class ConcatenationParser : public ParserInterface<TokenType>
{
public:
	/// Constructor. Takes parsers to concatenate.
	ConcatenationParser(const ParserRef<TokenType> &parser1, const ParserRef<TokenType> &parser2)
		: mParser1(parser1), mParser2(parser2)
	{
	}

	QSharedPointer<ast::Node> parse(TokenStream<TokenType> &tokenStream
			, ParserContext<TokenType> &parserContext) const override
	{
		if (tokenStream.isEnd()) {
			parserContext.reportError(QObject::tr("Unexpected end of input"));
			return wrap(new TemporaryErrorNode());
		}

		if (mParser1->first().contains(tokenStream.next().token())) {
			const QSharedPointer<ast::Node> parser1Result = mParser1->parse(tokenStream, parserContext);
			const QSharedPointer<ast::Node> parser2Result = mParser2->parse(tokenStream, parserContext);

			if (parser1Result->is<TemporaryErrorNode>() || parser2Result->is<TemporaryErrorNode>()) {
				return wrap(new TemporaryErrorNode());
			}

			if (parser1Result->is<TemporaryDiscardableNode>() && parser2Result->is<TemporaryDiscardableNode>()) {
				const auto result = wrap(new TemporaryDiscardableNode());
				result->connect(parser1Result);
				result->connect(parser2Result);
				return result;
			} else if (parser1Result->is<TemporaryDiscardableNode>()) {
				return parser2Result;
			} else if (parser2Result->is<TemporaryDiscardableNode>()) {
				return parser1Result;
			} else {
				const auto result = wrap(new TemporaryPair(parser1Result, parser2Result));
				result->connect(parser1Result);
				result->connect(parser2Result);
				return result;
			}
		}

		parserContext.reportError(QObject::tr("Unexpected token"));
		return wrap(new TemporaryErrorNode());
	}

	QSet<TokenType> first() const override
	{
		return mParser1->first();
	}

private:
	ParserRef<TokenType> mParser1;
	ParserRef<TokenType> mParser2;
};

}
}
