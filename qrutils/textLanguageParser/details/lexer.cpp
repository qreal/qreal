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

	int absolutePosition = 0;
	int line = 0;
	int column = 0;
	Lexemes::Type candidate = Lexemes::whitespace;
	while (absolutePosition < input.length()) {
		QRegularExpressionMatch bestMatch;

		for (Lexemes::Type const lexeme : mLexemes.lexemes().keys()) {
			QRegularExpression const &regExp = mLexemes.lexemes().value(lexeme);

			QRegularExpressionMatch const &match = regExp.match(
					input
					, absolutePosition
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
			if (candidate != Lexemes::whitespace && candidate != Lexemes::newline) {
				ast::Range range(bestMatch.capturedStart(), line, column
						, bestMatch.capturedEnd() - 1, line, column + bestMatch.capturedLength() - 1);

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
