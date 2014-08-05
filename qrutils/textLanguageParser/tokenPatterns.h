#pragma once

#include <QtCore/QHash>
#include <QtCore/QRegularExpression>

#include "textLanguageParser/tokenType.h"

#include "qrutils/utilsDeclSpec.h"

namespace textLanguageParser {

/// A list of tokens that can be recognized by lexer and regexps corresponding to them. Allows customization of
/// concrete syntax by replacing default regexp for token with custom one.
/// By default, token patterns follow Lua 5.3 specification. When defining own patterns, note that keywords
/// must be recognized by identifier pattern (as they are basically identifiers that are treated specially by parser),
/// newlines and whitespaces are used for connection tracking and error recovery, so be cautious with them.
///
/// Also note that keywords and other tokens are treated differently by lexer so shall be redefined by two separate
/// methods --- redefine() and redefineKeyword(). Keywords are described as strings, other tokens --- as regexps.
/// Trying to redefine keyword as token or token as keyword will yield runtime error. Token types and keywords
/// are still in one enum because we want to process them uniformly by parser, so just be cautious.
class TokenPatterns {
public:
	TokenPatterns();

	/// Redefine pattern for given token to given regular expression. New regexp will be used by lexer insead of
	/// default one.
	void redefine(TokenType tokenType, QRegularExpression const &regExp);

	/// Redefine description for given keyword to given string. New string will be used by lexer insead of default one.
	void redefineKeyword(TokenType keywordType, QString const &keyword);

	/// Returns a list of all tokens with known patterns.
	QList<TokenType> allPatterns() const;

	/// Returns pattern for a given token.
	QRegularExpression tokenPattern(TokenType type) const;

	/// Returns a list of all keywords with known definitions.
	QList<TokenType> allKeywords() const;

	/// Returns definition for a given keyword.
	QString keywordPattern(TokenType type) const;

private:
	QHash<TokenType, QRegularExpression> mPatterns;
	QHash<TokenType, QString> mKeywords;
};

}
