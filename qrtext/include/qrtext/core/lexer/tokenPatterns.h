#pragma once

#include <QtCore/QHash>
#include <QtCore/QRegularExpression>

#include "qrutils/utilsDeclSpec.h"

namespace qrtext {
namespace core {
namespace lexer {

/// A list of tokens that can be recognized by lexer and regexps corresponding to them.
/// Note that keywords must be recognized by identifier pattern (as they are basically identifiers that are treated
/// specially by parser), newlines and whitespaces are used for connection tracking and error recovery, so be cautious
/// with them.
///
/// Also note that keywords and other tokens are treated differently by lexer so shall be defined by two separate
/// methods --- defineToken() and defineKeyword(). Keywords are described as strings, other tokens --- as regexps.
/// Trying to redefine keyword as token or token as keyword will yield runtime error. Token types and keywords
/// are still in one enum because we want to process them uniformly by parser, so just be cautious.
template <typename TokenType>
class TokenPatterns {
public:
	/// Redefine pattern for given token to given regular expression. New regexp will be used by lexer insead of
	/// default one.
	void defineToken(TokenType tokenType, QRegularExpression const &regExp)
	{
		mPatterns.insert(tokenType, regExp);
	}

	/// Redefine description for given keyword to given string. New string will be used by lexer insead of default one.
	void defineKeyword(TokenType keywordType, QString const &keyword)
	{
		mKeywords.insert(keywordType, keyword);
	}

	/// Returns a list of all tokens with known patterns.
	QList<TokenType> allPatterns() const
	{
		return mPatterns.keys();
	}

	/// Returns pattern for a given token.
	QRegularExpression tokenPattern(TokenType type) const
	{
		return mPatterns.value(type);
	}

	/// Returns a list of all keywords with known definitions.
	QList<TokenType> allKeywords() const
	{
		return mKeywords.keys();
	}

	/// Returns definition for a given keyword.
	QString keywordPattern(TokenType type) const
	{
		return mKeywords.value(type);
	}

private:
	QHash<TokenType, QRegularExpression> mPatterns;
	QHash<TokenType, QString> mKeywords;
};

}
}
}
