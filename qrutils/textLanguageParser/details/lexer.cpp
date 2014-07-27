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

	for (Lexemes::Type const lexeme : mLexemes.lexemes().keys()) {
		QRegularExpression const &regExp = mLexemes.lexemes().value(lexeme);
		if (!regExp.isValid()) {
			qDebug() << "Invalid regexp" << regExp.pattern();
		}
	}

	int absolutePosition = 0;
	int line = 0;
	int column = 0;
	while (absolutePosition < input.length()) {
		Lexemes::Type candidate = Lexemes::whitespace;
		QRegularExpressionMatch bestMatch;

		for (Lexemes::Type const lexeme : mLexemes.lexemes().keys()) {
			QRegularExpression const &regExp = mLexemes.lexemes().value(lexeme);

			QRegularExpressionMatch const &match = regExp.match(
					input
					, absolutePosition
					, QRegularExpression::NormalMatch
					, QRegularExpression::AnchoredMatchOption);

			if (match.hasMatch()) {
				if (match.capturedLength() > bestMatch.capturedLength()) {
					bestMatch = match;
					candidate = lexeme;
				}
			}
		}

		if (candidate != Lexemes::whitespace) {
			qDebug() << "Best match:" << bestMatch;
		}

		if (bestMatch.hasMatch()) {
			if (candidate != Lexemes::whitespace && candidate != Lexemes::newline) {
				ast::Range range(bestMatch.capturedStart(), line, column
						, bestMatch.capturedEnd() - 1, line, column + bestMatch.capturedLength() - 1);

				if (candidate == Lexemes::identifier) {
					candidate = checkForKeyword(bestMatch.capturedTexts()[0]);
				}

				result.tokens.append(Token(candidate, range, bestMatch.capturedTexts()[0]));
			}

			if (candidate == Lexemes::newline) {
				++line;
				column = 0;
			} else {
				column += bestMatch.capturedLength();
			}

			absolutePosition += bestMatch.capturedLength();
		} else {
			/// @todo Report error.
			++absolutePosition;
		}
	}

	return result;
}

Lexemes::Type Lexer::checkForKeyword(QString const &identifier) const
{
	for (Lexemes::Type const keyword : mLexemes.keywords().keys()) {
		if (mLexemes.keywords()[keyword] == identifier) {
			return keyword;
		}
	}

	return Lexemes::identifier;
}
