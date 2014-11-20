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
		auto const ast = mGrammar->parse(*mTokenStream, *mContext);
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
