#pragma once

#include <QtCore/QHash>
#include <QtCore/QRegularExpression>

#include "qrutils/utilsDeclSpec.h"

namespace textLanguageParser {

class Lexemes {
public:
	Lexemes();

	enum Type {
		whitespace
		, newline
		, identifier

		// Keywords
		, andKeyword
		, breakKeyword
		, doKeyword
		, elseKeyword
		, elseifKeyword
		, endKeyword
		, falseKeyword
		, forKeyword
		, functionKeyword
		, gotoKeyword
		, ifKeyword
		, inKeyword
		, localKeyword
		, nilKeyword
		, notKeyword
		, orKeyword
		, repeatKeyword
		, returnKeyword
		, thenKeyword
		, trueKeyword
		, untilKeyword
		, whileKeyword

		, plus
		, minus
		, asterick
		, slash
		, percent
		, hat
		, sharp

		, ampersand
		, tilda
		, verticalLine
		, doubleLess
		, doubleGreater
		, doubleSlash

		, doubleEquals
		, tildaEquals
		, lessEquals
		, greaterEquals
		, less
		, greater
		, equals

		, openingBracket
		, closingBracket
		, openingCurlyBracket
		, closingCurlyBracket
		, openingSquareBracket
		, closingSquareBracket
		, doubleColon

		, semicolon
		, colon
		, comma
		, dot
		, doubleDot
		, tripleDot

		, string
		, integerLiteral
		, floatLiteral
		, comment
	};

	void redefine(Type lexemeType, QRegularExpression const &regExp);
	void redefineKeyword(Type keywordType, QString const &keyword);

	/// @todo Hide it.
	QHash<Type, QRegularExpression> const &lexemes() const;

	QHash<Type, QString> const &keywords() const;

private:
	QHash<Type, QRegularExpression> mLexemes;
	QHash<Type, QString> mKeywords;
};

}
