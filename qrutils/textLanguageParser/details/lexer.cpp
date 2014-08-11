#include "textLanguageParser/details/lexer.h"

#include <QtCore/QDebug>

using namespace textLanguageParser;
using namespace textLanguageParser::details;

Lexer::Lexer(TokenPatterns const &patterns)
	: mPatterns(patterns)
{
	// Doing syntax check of lexeme regexps and searching for whitespace and newline definitions, they will be needed
	// later for error recovery.
	for (TokenType const tokenType : mPatterns.allPatterns()) {
		QRegularExpression const &regExp = mPatterns.tokenPattern(tokenType);
		if (!regExp.isValid()) {
			qDebug() << "Invalid regexp: " + regExp.pattern();
			mPatternsErrors << ParserError(ast::Connection(), "Invalid regexp: " + regExp.pattern()
					, ErrorType::lexicalError, Severity::internalError);
		} else {
			if (tokenType == TokenType::whitespace) {
				mWhitespaceRegexp = regExp;
			} else if (tokenType == TokenType::newline) {
				mNewLineRegexp = regExp;
			}
		}
	}
}

Lexer::Result Lexer::tokenize(QString const &input)
{
	Result result;

	result.errors << mPatternsErrors;

	// Initializing connection.
	int absolutePosition = 0;
	int line = 0;
	int column = 0;

	// Scanning input string, trying to find longest match with regexp from a list of lexemes.
	while (absolutePosition < input.length()) {
		CandidateMatch bestMatch = findBestMatch(input, absolutePosition);

		if (bestMatch.match.hasMatch()) {
			int tokenEndLine = line;
			int tokenEndColumn = column;

			if (bestMatch.candidate != TokenType::whitespace
					&& bestMatch.candidate != TokenType::newline
					&& bestMatch.candidate != TokenType::comment)
			{
				// Determining connection of the lexeme. String is the only token that can span multiple lines so
				// special care is needed to maintain connection.
				if (bestMatch.candidate == TokenType::string) {
					QRegularExpressionMatchIterator matchIterator = mNewLineRegexp.globalMatch(
							bestMatch.match.captured());

					QRegularExpressionMatch match;

					while (matchIterator.hasNext()) {
						match = matchIterator.next();
						++tokenEndLine;
					}

					if (match.hasMatch()) {
						int relativeLastNewLineOffset = match.capturedEnd() - 1;
						int absoluteLastNewLineOffset = absolutePosition + relativeLastNewLineOffset;
						int absoluteTokenEnd = bestMatch.match.capturedEnd() - 1;
						tokenEndColumn = absoluteTokenEnd - absoluteLastNewLineOffset - 1;
					} else {
						tokenEndColumn += bestMatch.match.capturedLength() - 1;
					}
				} else {
					tokenEndColumn += bestMatch.match.capturedLength() - 1;
				}

				ast::Range range(bestMatch.match.capturedStart(), line, column
						, bestMatch.match.capturedEnd() - 1, tokenEndLine, tokenEndColumn);

				if (bestMatch.candidate == TokenType::identifier) {
					// Keyword is an identifier which is separate lexeme.
					bestMatch.candidate = checkForKeyword(bestMatch.match.captured());
				}

				result.tokens << Token(bestMatch.candidate, range, bestMatch.match.captured());
			} else if (bestMatch.candidate == TokenType::comment) {
				tokenEndColumn += bestMatch.match.capturedLength() - 1;
				ast::Range range(bestMatch.match.capturedStart(), line, column
						, bestMatch.match.capturedEnd() - 1, tokenEndLine, tokenEndColumn);

				result.comments << Token(bestMatch.candidate, range, bestMatch.match.captured());
			}

			// Keeping connection updated.
			if (bestMatch.candidate == TokenType::newline) {
				++line;
				column = 0;
			} else if (bestMatch.candidate == TokenType::whitespace || bestMatch.candidate == TokenType::comment) {
				column += bestMatch.match.capturedLength();
			} else {
				line = tokenEndLine;
				column = tokenEndColumn + 1;
			}

			absolutePosition += bestMatch.match.capturedLength();
		} else {
			result.errors << ParserError({absolutePosition, line, column}
					, "Lexer error", ErrorType::lexicalError, Severity::error);

			// Panic mode: syncing on nearest whitespace or newline token.
			while (!mWhitespaceRegexp.match(input, absolutePosition
					, QRegularExpression::NormalMatch, QRegularExpression::AnchoredMatchOption).hasMatch()
					&& !mNewLineRegexp.match(input, absolutePosition
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

Lexer::CandidateMatch Lexer::findBestMatch(QString const &input, int const absolutePosition) const
{
	TokenType candidate = TokenType::whitespace;
	QRegularExpressionMatch bestMatch;

	for (TokenType const token : mPatterns.allPatterns()) {
		QRegularExpression const &regExp = mPatterns.tokenPattern(token);

		QRegularExpressionMatch const &match = regExp.match(
				input
				, absolutePosition
				, QRegularExpression::NormalMatch
				, QRegularExpression::AnchoredMatchOption);

		if (match.hasMatch()) {
			if (match.capturedLength() > bestMatch.capturedLength()) {
				bestMatch = match;
				candidate = token;
			}
		}
	}

	return CandidateMatch{candidate, bestMatch};
}

TokenType Lexer::checkForKeyword(QString const &identifier) const
{
	for (TokenType const keyword : mPatterns.allKeywords()) {
		if (mPatterns.keywordPattern(keyword) == identifier) {
			return keyword;
		}
	}

	return TokenType::identifier;
}
