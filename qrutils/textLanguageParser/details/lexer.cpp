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

	for (Lexemes::Type const lexeme : mLexemes.allLexemes()) {
		QRegularExpression const &regExp = mLexemes.lexemeDefinition(lexeme);
		if (!regExp.isValid()) {
			qDebug() << "Invalid regexp: " + regExp.pattern();
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

		for (Lexemes::Type const lexeme : mLexemes.allLexemes()) {
			QRegularExpression const &regExp = mLexemes.lexemeDefinition(lexeme);

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
			int tokenEndLine = line;
			int tokenEndColumn = column;

			if (candidate != Lexemes::whitespace && candidate != Lexemes::newline && candidate != Lexemes::comment)
			{
				// Determining connection of the lexeme. String is the only token that can span multiple lines so
				// special care is needed to maintain connection.
				if (candidate == Lexemes::string) {
					QRegularExpressionMatchIterator matchIterator = newLineRegexp.globalMatch(
							bestMatch.captured());

					QRegularExpressionMatch match;

					while (matchIterator.hasNext()) {
						match = matchIterator.next();
						++tokenEndLine;
					}

					if (match.hasMatch()) {
						int relativeLastNewLineOffset = match.capturedEnd() - 1;
						int absoluteLastNewLineOffset = absolutePosition + relativeLastNewLineOffset;
						int absoluteTokenEnd = bestMatch.capturedEnd() - 1;
						tokenEndColumn = absoluteTokenEnd - absoluteLastNewLineOffset - 1;
					} else {
						tokenEndColumn += bestMatch.capturedLength() - 1;
					}
				} else {
					tokenEndColumn += bestMatch.capturedLength() - 1;
				}

				ast::Range range(bestMatch.capturedStart(), line, column
						, bestMatch.capturedEnd() - 1, tokenEndLine, tokenEndColumn);

				if (candidate == Lexemes::identifier) {
					// Keyword is an identifier which is separate lexeme.
					candidate = checkForKeyword(bestMatch.captured());
				}

				result.tokens << Token(candidate, range, bestMatch.captured());
			} else if (candidate == Lexemes::comment) {
				tokenEndColumn += bestMatch.capturedLength() - 1;
				ast::Range range(bestMatch.capturedStart(), line, column
						, bestMatch.capturedEnd() - 1, tokenEndLine, tokenEndColumn);

				result.comments << Token(candidate, range, bestMatch.captured());
			}

			// Keeping connection updated.
			if (candidate == Lexemes::newline) {
				++line;
				column = 0;
			} else if (candidate == Lexemes::whitespace || candidate == Lexemes::comment) {
				column += bestMatch.capturedLength();
			} else {
				line = tokenEndLine;
				column = tokenEndColumn + 1;
			}

			absolutePosition += bestMatch.capturedLength();
		} else {
			result.errors << ParserError({absolutePosition, line, column}
					, "Lexer error", ParserError::lexicalError, ParserError::error);

			// Panic mode: syncing on nearest whitespace or newline token.
			while (!whitespaceRegexp.match(input, absolutePosition
					, QRegularExpression::NormalMatch, QRegularExpression::AnchoredMatchOption).hasMatch()
					&& !newLineRegexp.match(input, absolutePosition
							, QRegularExpression::NormalMatch, QRegularExpression::AnchoredMatchOption).hasMatch()
					&& absolutePosition < input.length())
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
	for (Lexemes::Type const keyword : mLexemes.allKeywords()) {
		if (mLexemes.keywordDefinition(keyword) == identifier) {
			return keyword;
		}
	}

	return Lexemes::identifier;
}
