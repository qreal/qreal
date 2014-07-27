#pragma once

#include <QtCore/QHash>
#include <QtCore/QRegularExpression>

#include "qrutils/utilsDeclSpec.h"

namespace textLanguageParser {

/// A list of lexemes that can be recognized by lexer and regexps corresponding to them. Allows customization of
/// concrete syntax by replacing default regexp for lexeme with custom one.
/// By default, lexeme definitions follow Lua 5.3 specification. When defining own lexemes, note that keywords
/// must be recognized by identifier pattern (as they are basically identifiers that are treated specially by parser),
/// newlines and whitespaces are used for connection tracking and error recovery, so be cautious with them.
///
/// Also note that keywords and other lexemes are treated differently by lexer so shall be redefined by two separate
/// methods --- redefine() and redefineKeyword(). Keywords are described as strings, other lexemes --- as regexps.
/// Trying to redefine keyword as lexeme or lexeme as keyword will yield runtime error. Lexemes and keywords
/// are still in one enum because we want to process them uniformly by parser, so just be cautious.
class Lexemes {
public:
	Lexemes();

	enum Type {
		whitespace
		, newline
		, identifier

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

	/// Redefine description for given lexeme to given regular expression. New regexp will be used by lexer insead of
	/// default one.
	void redefine(Type lexemeType, QRegularExpression const &regExp);

	/// Redefine description for given keyword to given string. New string will be used by lexer insead of default one.
	void redefineKeyword(Type keywordType, QString const &keyword);

	/// Returns a list of all lexemes with known definitions.
	QList<Type> allLexemes() const;

	/// Returns definition for a given lexeme.
	QRegularExpression lexemeDefinition(Type type) const;

	/// Returns a list of all keywords with known definitions.
	QList<Type> allKeywords() const;

	/// Returns definition for a given keyword.
	QString keywordDefinition(Type type) const;

private:
	QHash<Type, QRegularExpression> mLexemes;
	QHash<Type, QString> mKeywords;
};

}
