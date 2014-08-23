#pragma once

#include <QtCore/QHash>
#include <QtCore/QSet>

namespace qrtext {
namespace core {

enum class Associativity {
	left
	, right
};

enum class Arity {
	unary
	, binary
};

template<typename TokenType>
class PrecedenceTable
{
public:
	virtual ~PrecedenceTable() {}

	int precedence(TokenType token, Arity arity) const
	{
		if (arity == Arity::binary) {
			return mBinaryOperatorPrecedences[token].first;
		} else {
			return mUnaryOperatorPrecedences[token];
		}
	}

	Associativity associativity(TokenType token) const
	{
		return mBinaryOperatorPrecedences[token].second;
	}

	QSet<TokenType> binaryOperators() const
	{
		return QSet<TokenType>::fromList(mBinaryOperatorPrecedences.keys());
	}

protected:
	void addOperator(TokenType token, int precedence, Associativity associativity)
	{
		mBinaryOperatorPrecedences.insert(token, {precedence, associativity});
	}

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
