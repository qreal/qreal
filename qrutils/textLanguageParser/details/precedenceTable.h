#pragma once

#include <QtCore/QHash>
#include <QtCore/QSet>

#include "textLanguageParser/tokenType.h"

namespace textLanguageParser {
namespace details {

enum class Associativity {
	left
	, right
};

class PrecedenceTable
{
public:
	PrecedenceTable();

	int precedence(TokenType token) const;

	Associativity associativity(TokenType token) const;

	QSet<TokenType> binaryOperators() const;

	static int unaryOperatorsPrecedence();

private:
	QHash<TokenType, QPair<int, Associativity>> mPrecedences;
};

}
}
