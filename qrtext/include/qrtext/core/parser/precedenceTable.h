#pragma once

#include <QtCore/QHash>
#include <QtCore/QSet>

namespace qrtext {
namespace core {

/// Associativity of binary operator. For example, "+" is usually left associative, i.e. 1+2+3 is parsed as (1+2)+3.
enum class Associativity
{
	left
	, right
};

/// Arity of operator. Now qrtext supports only unary and binary operators.
enum class Arity
{
	unary
	, binary
};

/// Provides information about associativity and precedence of operators to expression parser. Stores binary and unary
/// operator precedences. Typical usage pattern is to subclass this class for concrete language and fill precedence
/// table calling addOperator() in constructor.
template<typename TokenType>
class PrecedenceTable
{
public:
	virtual ~PrecedenceTable() {}

	/// Returns precedence of an operator denoted by its token type and arity. Bigger precedence numbers correspond
	/// to higher precedence.
	int precedence(TokenType token, Arity arity) const
	{
		if (arity == Arity::binary) {
			return mBinaryOperatorPrecedences[token].first;
		} else {
			return mUnaryOperatorPrecedences[token];
		}
	}

	/// Returns associativity of binary operator.
	Associativity associativity(TokenType token) const
	{
		return mBinaryOperatorPrecedences[token].second;
	}

	/// Returns a set of all known binary operators (their token types).
	QSet<TokenType> binaryOperators() const
	{
		return QSet<TokenType>::fromList(mBinaryOperatorPrecedences.keys());
	}

protected:
	/// Adds binary operator with given precedence and associativity to a table.
	void addOperator(TokenType token, int precedence, Associativity associativity)
	{
		mBinaryOperatorPrecedences.insert(token, {precedence, associativity});
	}

	/// Adds unary operator with given precedence to a table.
	void addOperator(TokenType token, int precedence)
	{
		mUnaryOperatorPrecedences.insert(token, precedence);
	}

private:
	QHash<TokenType, QPair<int, Associativity>> mBinaryOperatorPrecedences;
	QHash<TokenType, int> mUnaryOperatorPrecedences;
};

}
}
