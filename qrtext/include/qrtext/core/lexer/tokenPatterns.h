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

#include <QtCore/QHash>
#include <QtCore/QRegularExpression>

#include "qrutils/utilsDeclSpec.h"

namespace qrtext {
namespace core {

/// A list of tokens that can be recognized by lexer and regexps corresponding to them.
/// Note that keywords must be recognized by identifier pattern (as they are basically identifiers that are treated
/// specially by parser), newlines and whitespaces are used for connection tracking and error recovery, so be cautious
/// with them.
///
/// Also note that keywords and other tokens are treated differently by lexer so shall be defined by two separate
/// methods --- defineToken() and defineKeyword(). Keywords are described as strings, other tokens --- as regexps.
/// Token types and keywords are still in one enum because we want to process them uniformly by parser.
template <typename TokenType>
class TokenPatterns
{
public:
	/// Define pattern for given token to given regular expression with given display name.
	void defineToken(TokenType tokenType, const QRegularExpression &regExp, const QString &userFriendlyName)
	{
		mPatterns.insert(tokenType, regExp);
		mUserFriendlyNames.insert(tokenType, userFriendlyName);
	}

	/// Description for given keyword to given string.
	void defineKeyword(TokenType keywordType, const QString &keyword)
	{
		mKeywords.insert(keywordType, keyword);
		mUserFriendlyNames.insert(keywordType, keyword);
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

	/// Returns a table of user-friendly names for tokens.
	QHash<TokenType, QString> tokenUserFriendlyNames() const
	{
		return mUserFriendlyNames;
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
	QHash<TokenType, QString> mUserFriendlyNames;
	QHash<TokenType, QString> mKeywords;
};

}
}
