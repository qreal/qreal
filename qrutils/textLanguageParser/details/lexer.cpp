#include "textLanguageParser/details/lexer.h"

#include <QtCore/QDebug>

using namespace textLanguageParser;
using namespace textLanguageParser::details;

Lexer::Lexer(Lexemes const &lexemes)
	: mLexemes(lexemes)
{
}

Lexer::Result Lexer::tokenize(QString const &input)
{
	Result result;

	int position = 0;
	Lexemes::Type candidate = Lexemes::whitespace;
	while (position < input.length()) {
		QRegularExpressionMatch bestMatch;

		for (Lexemes::Type const lexeme : mLexemes.lexemes().keys()) {
			QRegularExpression const &regExp = mLexemes.lexemes().value(lexeme);

			QRegularExpressionMatch const &match = regExp.match(
					input
					, position
					, QRegularExpression::NormalMatch
					, QRegularExpression::AnchoredMatchOption);

			qDebug() << regExp.pattern() << match;

			if (match.hasMatch()) {
				if (match.capturedLength() > bestMatch.capturedLength()) {
					bestMatch = match;
					candidate = lexeme;
				}
			}
		}

		qDebug() << "Best match:" << bestMatch;

		if (bestMatch.hasMatch()) {
			ast::Connection connection;
			if (candidate != Lexemes::whitespace) {
				result.tokens.append(Token(candidate, connection, bestMatch.capturedTexts()[0]));
			}

			position += bestMatch.capturedLength();
		} else {
			/// @todo Report error.
			++position;
		}
	}

	return result;
}
