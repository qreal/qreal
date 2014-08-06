#include "textLanguageParser/details/precedenceTable.h"

using namespace textLanguageParser;
using namespace textLanguageParser::details;

PrecedenceTable::PrecedenceTable()
{
	mPrecedences[TokenType::orKeyword] = {0, Associativity::left};

	mPrecedences[TokenType::andKeyword] = {1, Associativity::left};

	mPrecedences[TokenType::less] = {2, Associativity::left};
	mPrecedences[TokenType::greater] = {2, Associativity::left};
	mPrecedences[TokenType::lessEquals] = {2, Associativity::left};
	mPrecedences[TokenType::greaterEquals] = {2, Associativity::left};
	mPrecedences[TokenType::tildaEquals] = {2, Associativity::left};
	mPrecedences[TokenType::equals] = {2, Associativity::left};

	mPrecedences[TokenType::verticalLine] = {3, Associativity::left};

	// Note that there is also unary tilda, it has hardcoded precedence, as all other unary operators.
	mPrecedences[TokenType::tilda] = {4, Associativity::left};

	mPrecedences[TokenType::ampersand] = {5, Associativity::left};

	mPrecedences[TokenType::doubleLess] = {6, Associativity::left};
	mPrecedences[TokenType::doubleGreater] = {6, Associativity::left};

	mPrecedences[TokenType::doubleDot] = {7, Associativity::right};

	mPrecedences[TokenType::plus] = {8, Associativity::left};
	mPrecedences[TokenType::minus] = {8, Associativity::left};

	mPrecedences[TokenType::asterick] = {9, Associativity::left};
	mPrecedences[TokenType::slash] = {9, Associativity::left};
	mPrecedences[TokenType::doubleSlash] = {9, Associativity::left};
	mPrecedences[TokenType::percent] = {9, Associativity::left};

	// TokenType::notKeyword, TokenType::sharp, TokenType::minus, TokenType::tilda are unary operators and processed
	// separately. Their precedence is 10.

	mPrecedences[TokenType::hat] = {11, Associativity::right};
}

int PrecedenceTable::precedence(TokenType token) const
{
	return mPrecedences[token].first;
}

Associativity PrecedenceTable::associativity(TokenType token) const
{
	return mPrecedences[token].second;
}

QSet<TokenType> PrecedenceTable::binaryOperators() const
{
	return QSet<TokenType>::fromList(mPrecedences.keys());
}

int PrecedenceTable::unaryOperatorsPrecedence()
{
	// Hardcoded, because unary operators may have the same lexemes as binary operators, so there is no way to differ
	// them at lexical level. Using AST nodes here maybe more correct, but inconvenient.
	return 10;
}
