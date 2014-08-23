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

template<typename TokenType>
class Parser {
public:
	explicit Parser(QSharedPointer<ParserInterface<TokenType>> const &grammar, QList<Error> &errors)
		: mErrors(errors), mGrammar(grammar)
	{
	}

	/// Parses given code and returns AST with results or nullptr if parsing is impossible.
	QSharedPointer<ast::Node> parse(QList<Token<TokenType>> const &tokens)
	{
		TokenStream<TokenType> tokenStream(tokens, mErrors);
		ParserContext<TokenType> context(mErrors, tokenStream);
		return mGrammar->parse(tokenStream, context);
	}

protected:
	ParserContext<TokenType> &context()
	{
		return *mContext;
	}

private:
	QList<Error> &mErrors;
	QSharedPointer<ParserInterface<TokenType>> mGrammar;
	QScopedPointer<ParserContext<TokenType>> mContext;
};

}
}
