#include "textLanguageParser.h"

#include "details/lexer.h"
#include "tokenType.h"

using namespace textLanguageParser;

TextLanguageParser::TokenStream::TokenStream(QList<details::Token> const &tokenList, QList<ParserError> &errorList)
	: mTokenList(tokenList), mErrorList(errorList), mPosition(0)
{
}

details::Token TextLanguageParser::TokenStream::next()
{
	return mTokenList.at(mPosition);
}

void TextLanguageParser::TokenStream::consume()
{
	++mPosition;
}

bool TextLanguageParser::TokenStream::expect(TokenType token)
{
	if (next().token() == token) {
		consume();
		return true;
	} else {
		mErrorList << ParserError(next().range().start()
				, QString("Expected %1, got %2").arg(static_cast<int>(token)).arg(static_cast<int>(next().token()))
				, ErrorType::syntaxError
				, Severity::error);

		return false;
	}
}

TextLanguageParserInterface::Result TextLanguageParser::parse(QString const &code)
{
	TokenPatterns lexemes;
	details::Lexer lexer(lexemes);

	details::Lexer::Result lexerResult = lexer.tokenize(code);
	TokenStream tokenStream(lexerResult.tokens, lexerResult.errors);

	Result result(nullptr, lexerResult.errors);

	return result;
}
