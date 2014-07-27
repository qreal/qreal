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

	// Doing syntax check of lexeme regexps and searching for whitespace and newline definitions, they will be needed
	// later for error recovery.
	/// @todo Remove initialization and sanity check to some other place.
	QRegularExpression whitespaceRegexp;
	QRegularExpression newLineRegexp;

	for (Lexemes::Type const lexeme : mLexemes.lexemes().keys()) {
		QRegularExpression const &regExp = mLexemes.lexemes().value(lexeme);
		if (!regExp.isValid()) {
			result.errors << ParserError(ast::Connection(), "Invalid regexp: " + regExp.pattern()
					, ParserError::lexicalError, ParserError::internalError);
		} else {
			if (lexeme == Lexemes::whitespace) {
				whitespaceRegexp = regExp;
			} else if (lexeme == Lexemes::newline) {
				newLineRegexp = regExp;
			}
		}
	}

	// Initializing connection.
	int absolutePosition = 0;
	int line = 0;
	int column = 0;

	// Scanning input string, trying to find longest match with regexp from a list of lexemes.
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
				// Determining connection of the lexeme. Multiline tokens like strings and long comments are not
				// supported yet, so we can safely assume that a token starts and ends on one line.
				ast::Range range(bestMatch.capturedStart(), line, column
						, bestMatch.capturedEnd() - 1, line, column + bestMatch.capturedLength() - 1);

				if (candidate == Lexemes::identifier) {
					// Keyword is an identifier which is separate lexeme.
					candidate = checkForKeyword(bestMatch.capturedTexts()[0]);
				}

				result.tokens << Token(candidate, range, bestMatch.capturedTexts()[0]);
			}

			// Keeping connection updated.
			if (candidate == Lexemes::newline) {
				++line;
				column = 0;
			} else {
				column += bestMatch.capturedLength();
			}

			absolutePosition += bestMatch.capturedLength();
		} else {
			result.errors << ParserError({absolutePosition, line, column}
					, "Lexer error", ParserError::lexicalError, ParserError::error);

			// Panic mode: syncing on nearest whitespace or newline token.
			while (!whitespaceRegexp.match(input, absolutePosition
					, QRegularExpression::NormalMatch, QRegularExpression::AnchoredMatchOption).hasMatch()
					&& !newLineRegexp.match(input, absolutePosition
							, QRegularExpression::NormalMatch, QRegularExpression::AnchoredMatchOption).hasMatch())
			{
				++absolutePosition;
				++column;
			}
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
