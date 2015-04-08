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

#include "qrtext/core/parser/operators/parserInterface.h"
#include "qrtext/core/parser/temporaryNodes/temporaryErrorNode.h"
#include "qrtext/core/parser/temporaryNodes/temporaryDiscardableNode.h"

namespace qrtext {
namespace core {

/// Elementary parser that parses given token and executes given semantic action on a result. Semantic action takes
/// no parameters.
template<typename TokenType, typename SemanticAction>
class SimpleParser : public ParserInterface<TokenType>
{
public:
	/// Constructor. Takes token to parse and lambda function to execute if token is parsed successfully.
	SimpleParser(TokenType token, const SemanticAction &semanticAction)
		: mToken(token), mSemanticAction(semanticAction)
	{
	}

	QSharedPointer<ast::Node> parse(TokenStream<TokenType> &tokenStream
			, ParserContext<TokenType> &parserContext) const override
	{
		Q_UNUSED(parserContext);

		Token<TokenType> const token = tokenStream.next();
		if (!tokenStream.expect(mToken)) {
			return wrap(new TemporaryErrorNode());
		}

		const auto node = wrap(mSemanticAction());
		if (node) {
			node->connect(token);
		} else {
			parserContext.reportInternalError(
					QObject::tr("Semantic action incorrectly discarded node in SimpleParser"));

			return wrap(new TemporaryDiscardableNode());
		}

		return node;
	}

	QSet<TokenType> first() const override
	{
		return {mToken};
	}

private:
	TokenType mToken;
	SemanticAction mSemanticAction;
};

}
}
