#pragma once

#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QSet>

#include "textLanguageParser/details/token.h"
#include "textLanguageParser/parserError.h"
#include "textLanguageParser/tokenPatterns.h"

namespace textLanguageParser {
namespace details {

/// Configurable lexer of something like Lua 5.3 based on regular expressions. Takes token patterns, provides
/// a list of tokens by given input string, list of lexer errors and a separate list of comments. Allows Unicode input
/// and lexeme definitions.
/// Requires that token redefinitions are sane, for example, keywords shall match the definition of identifiers.
/// Does syntax check on passed regexps (and reports errors as InternalError error type), but does not check whole set
/// of patterns for consistensy. Pattern match order is arbitrary, so there shall be no ambiquities in pattern
///  definitions (except between keywords and identifiers, keywords are processed separately). Does not add whitespaces
/// and newlines to output token stream, but does use them for connection and error recovery, so it is recommended to
/// not fiddle with them much.
/// In case of error skips symbols until next whitespace or newline and reports error.
/// Can be quite slow due to use of regexp matching with every regexp in patterns list, so do not use this lexer
/// on really large files.
///
/// Now lexer (with default token patterns) follows Lua 5.3 specification with following exceptions:
/// - long brackets are not supported, either for string literals or for comments.
class Lexer {
public:
	/// Result of tokenization.
	struct Result {
		/// A list of recognized tokens.
		QList<Token> tokens;

		/// A list of detected lexer errors.
		QList<ParserError> errors;

		/// A list of comments.
		QList<Token> comments;
	};

	/// Constructor.
	/// @param patterns - object containing token patterns.
	explicit Lexer(TokenPatterns const &patterns);

	/// Tokenizes input string, returns list of detected tokens, list of errors and separate list of comments.
	Result tokenize(QString const &input);

private:
	struct CandidateMatch {
		TokenType candidate;
		QRegularExpressionMatch match;
	};

	TokenType checkForKeyword(QString const &identifier) const;
	CandidateMatch findBestMatch(const QString &input, int const absolutePosition) const;

	TokenPatterns const mPatterns;
	QRegularExpression mWhitespaceRegexp;
	QRegularExpression mNewLineRegexp;

	QList<ParserError> mPatternsErrors;
};

}
}
