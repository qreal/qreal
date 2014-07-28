#pragma once

#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QSet>

#include "textLanguageParser/details/token.h"
#include "textLanguageParser/parserError.h"
#include "textLanguageParser/lexemes.h"

namespace textLanguageParser {
namespace details {

/// Configurable lexer of something like Lua 5.3 based on regular expressions. Takes lexemes description, provides
/// a list of tokens by given input string, list of lexer errors and a separate list of comments. Allows Unicode input
/// and lexeme definitions.
/// Requires that lexeme redefinitions are not insane, for example, keywords shall match the definition of identifiers.
/// Does syntax check on passed regexps (and reports errors as InternalError error type), but does not check whole set
/// of lexemes for consistensy. Lexeme match order is arbitrary, so there shall be no ambiquities in lexeme definitions
/// (except between keywords and identifiers, keywords are processed separately). Does not add whitespaces and newlines
/// to output token stream, but does use them for connection and error recovery, so it is recommended to not fiddle with
/// them much.
/// In case of error skips symbols until next whitespace or newline and reports error.
/// Can be quite slow due to use of regexp matching with every regexp in lexemes description, so do not use this lexer
/// on really large files.
///
/// Now lexer (with default lexemes definition) follows Lua 5.3 specification with some exceptions:
/// - comments are not supported
/// - literal strings are not supported
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
	/// @param lexemes - lexemes definition object.
	explicit Lexer(Lexemes const &lexemes);

	/// Tokenizes input string, returns list of detected tokens, list of errors and separate list of comments.
	Result tokenize(QString const &input);

private:
	Lexemes::Type checkForKeyword(QString const &identifier) const;

	Lexemes const mLexemes;
};

}
}
