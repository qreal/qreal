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

#include <QtCore/QSharedPointer>

#include "qrtext/core/error.h"
#include "qrtext/core/ast/node.h"
#include "qrtext/core/lexer/token.h"
#include "qrtext/core/parser/tokenStream.h"
#include "qrtext/core/parser/parserContext.h"
#include "qrtext/core/parser/operators/parserInterface.h"

#include "qrtext/declSpec.h"

namespace qrtext {
namespace core {

/// Generic parser. Uses grammar that is passed to constructor to parse a stream of tokens. Typical use case is to
/// subclass this class for concrete language and provide its grammar in constructor of a subclass.
template<typename TokenType>
class Parser
{
public:
	/// Constructor. Takes grammar of a language to parse in form of a ParserInterface and a list of errors where to put
	/// parser errors.
	explicit Parser(QSharedPointer<ParserInterface<TokenType>> const &grammar, QList<Error> &errors)
		: mErrors(errors), mGrammar(grammar)
	{
	}

	/// Parses given stream of tokens and returns AST with results or nullptr if parsing is impossible.
	/// @param tokens - a stream of tokens to parse.
	/// @param tokenUserFriendlyNames - map with displayed names of tokens.
	QSharedPointer<ast::Node> parse(QList<Token<TokenType>> const &tokens
			, QHash<TokenType, QString> const &tokenUserFriendlyNames)
	{
		mTokenStream.reset(new TokenStream<TokenType>(tokens, tokenUserFriendlyNames, mErrors));
		mContext.reset(new ParserContext<TokenType>(mErrors, *mTokenStream));
		const auto ast = mGrammar->parse(*mTokenStream, *mContext);
		if (!mTokenStream->isEnd()) {
			mContext->reportError(QObject::tr("Unexpected token"));
		}

		return ast;
	}

protected:
	/// Provides access to parser context for subclasses.
	ParserContext<TokenType> &context()
	{
		return *mContext;
	}

private:
	QList<Error> &mErrors;
	QSharedPointer<ParserInterface<TokenType>> mGrammar;
	QScopedPointer<ParserContext<TokenType>> mContext;
	QScopedPointer<TokenStream<TokenType>> mTokenStream;
};

}
}
