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

#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QSet>

#include "qrtext/core/lexer/token.h"
#include "qrtext/core/error.h"
#include "qrtext/core/lexer/tokenPatterns.h"

namespace qrtext {
namespace core {

/// Configurable lexer based on regular expressions. Takes token patterns, provides a list of tokens by given input
/// string, list of lexer errors and a separate list of comments. Allows Unicode input and lexeme definitions.
/// Requires that token redefinitions are sane, for example, keywords shall match the definition of identifiers.
/// Performs syntax check on passed regexps (and reports errors as InternalError error type), but does not check whole
/// set of patterns for consistensy. Pattern match order is arbitrary, so there shall be no ambiquities in pattern
/// definitions (except between keywords and identifiers, keywords are processed separately). Does not add whitespaces
/// and newlines to output token stream, but does use them for connection and error recovery, so it is recommended to
/// not fiddle with them much.
/// In case of error skips symbols until next whitespace or newline and reports error.
/// Can be quite slow due to use of regexp matching with every regexp in patterns list, so do not use this lexer
/// on really large files.
///
/// It is parameterized by TokenType --- enum class with all token types of a language. Token types may be arbitrary,
/// but shall always contain TokenType::whitespace, TokenType::newline, TokenType::comment, TokenType::string,
/// TokenType::identifier, othrewise template instantiation will fail.
/// TokenType::comment shall be the only token that can span multiple lines, so multiline comments are not supported.
template<typename TokenType>
class Lexer
{
public:
	/// Constructor.
	/// @param patterns - object containing token patterns.
	/// @param errors - error stream, to which lexer errors shall be added.
	explicit Lexer(TokenPatterns<TokenType> const &patterns, QList<Error> &errors)
		: mPatterns(patterns), mErrors(errors)
	{
		// Doing syntax check of lexeme regexps and searching for whitespace and newline definitions, they will be
		// needed later for error recovery.
		for (const TokenType tokenType : mPatterns.allPatterns()) {
			const QRegularExpression &regExp = mPatterns.tokenPattern(tokenType);
			if (!regExp.isValid()) {
				qDebug() << "Invalid regexp: " + regExp.pattern();
				mErrors << Error(Connection(), QObject::tr("Invalid regexp: ") + regExp.pattern()
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

	/// Tokenizes input string, returns list of detected tokens, list of errors and separate list of comments.
	QList<Token<TokenType>> tokenize(const QString &input)
	{
		mComments.clear();

		QList<Token<TokenType>> result;

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
							const int relativeLastNewLineOffset = match.capturedEnd() - 1;
							const int absoluteLastNewLineOffset = absolutePosition + relativeLastNewLineOffset;
							const int absoluteTokenEnd = bestMatch.match.capturedEnd() - 1;
							tokenEndColumn = absoluteTokenEnd - absoluteLastNewLineOffset - 1;
						} else {
							tokenEndColumn += bestMatch.match.capturedLength() - 1;
						}
					} else {
						tokenEndColumn += bestMatch.match.capturedLength() - 1;
					}

					const Range range(Connection(bestMatch.match.capturedStart(), line, column)
							, Connection(bestMatch.match.capturedEnd() - 1, tokenEndLine, tokenEndColumn));

					if (bestMatch.candidate == TokenType::identifier) {
						// Keyword is an identifier which is separate lexeme.
						bestMatch.candidate = checkForKeyword(bestMatch.match.captured());
					}

					result << Token<TokenType>(bestMatch.candidate
							, range
							, bestMatch.match.captured());
				} else if (bestMatch.candidate == TokenType::comment) {
					tokenEndColumn += bestMatch.match.capturedLength() - 1;
					const Range range(Connection(bestMatch.match.capturedStart(), line, column)
							, Connection(bestMatch.match.capturedEnd() - 1, tokenEndLine, tokenEndColumn));

					mComments << Token<TokenType>(bestMatch.candidate
							, range
							, bestMatch.match.captured());
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
				const auto errorConnection = Connection(absolutePosition, line, column);
				QString skippedSymbols;

				// Panic mode: syncing on nearest whitespace or newline token.
				while (!mWhitespaceRegexp.match(input, absolutePosition
						, QRegularExpression::NormalMatch, QRegularExpression::AnchoredMatchOption).hasMatch()
						&& !mNewLineRegexp.match(input, absolutePosition
								, QRegularExpression::NormalMatch, QRegularExpression::AnchoredMatchOption).hasMatch()
						&& absolutePosition < input.length())
				{
					skippedSymbols += input.at(absolutePosition);
					++absolutePosition;
					++column;
				}

				mErrors << Error(errorConnection
						, QObject::tr("Unknown sequence of symbols: ") + skippedSymbols
						, ErrorType::lexicalError
						, Severity::error);
			}
		}

		return result;
	}

	/// Returns a list of comments from last tokenize() call.
	QList<Token<TokenType>> comments() const
	{
		return mComments;
	}

	/// Returns a map with user-friendly names of tokens.
	QHash<TokenType, QString> userFriendlyTokenNames() const
	{
		return mPatterns.tokenUserFriendlyNames();
	}

private:
	struct CandidateMatch {
		TokenType candidate;
		QRegularExpressionMatch match;
	};

	TokenType checkForKeyword(const QString &identifier) const
	{
		for (const TokenType keyword : mPatterns.allKeywords()) {
			if (mPatterns.keywordPattern(keyword) == identifier) {
				return keyword;
			}
		}

		return TokenType::identifier;
	}

	CandidateMatch findBestMatch(const QString &input, const int absolutePosition) const
	{
		TokenType candidate = TokenType::whitespace;
		QRegularExpressionMatch bestMatch;

		for (const TokenType token : mPatterns.allPatterns()) {
			const QRegularExpression &regExp = mPatterns.tokenPattern(token);

			const QRegularExpressionMatch &match = regExp.match(
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

	TokenPatterns<TokenType> const mPatterns;

	QRegularExpression mWhitespaceRegexp;
	QRegularExpression mNewLineRegexp;

	QList<Error> &mErrors;
	QList<Token<TokenType>> mComments;
};

}
}
