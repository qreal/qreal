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
#include "qrtext/core/parser/temporaryNodes/temporaryList.h"
#include "qrtext/core/parser/temporaryNodes/temporaryDiscardableNode.h"
#include "qrtext/core/parser/temporaryNodes/temporaryErrorNode.h"

namespace qrtext {
namespace core {

/// Elementary parser for parsing Kleene star, in form of A = B*. Temporary discardable nodes are discarded from
/// final output, but their connection preserved. Guaranteed to return TemporaryList (maybe empty).
template<typename TokenType>
class KleeneStarParser : public ParserInterface<TokenType>
{
public:
	/// Constructor. Takes parser for which to build Kleene closure.
	KleeneStarParser(const ParserRef<TokenType> &parser)
		: mParser(parser)
	{
	}

	QSharedPointer<ast::Node> parse(TokenStream<TokenType> &tokenStream
			, ParserContext<TokenType> &parserContext) const override
	{
		auto temporaryList = QSharedPointer<TemporaryList>(new TemporaryList());
		while (!tokenStream.isEnd() && mParser->first().contains(tokenStream.next().token())) {
			QSharedPointer<ast::Node> result = mParser->parse(tokenStream, parserContext);
			if (result->is<TemporaryErrorNode>()) {
				return result;
			}

			if (!result->is<TemporaryDiscardableNode>()) {
				temporaryList->list() << result;
			}

			temporaryList->connect(result);
		}

		return temporaryList;
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
