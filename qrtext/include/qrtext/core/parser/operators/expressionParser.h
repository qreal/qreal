#pragma once

#include "qrtext/core/ast/binaryOperator.h"
#include "qrtext/core/parser/operators/parserInterface.h"
#include "qrtext/core/parser/parserRef.h"
#include "qrtext/core/parser/precedenceTable.h"

namespace qrtext {
namespace core {
namespace parser {

template<typename TokenType>
class ExpressionParser : public ParserInterface<TokenType> {
public:
	ExpressionParser(ParserRef<TokenType> const &primary, ParserRef<TokenType> const &binOp)
		: mStartPrecedence(0)
		, mPrimary(primary)
		, mBinOp(binOp)
	{
	}

	ExpressionParser(TokenType startingUnaryOperator, ParserRef<TokenType> primary, ParserRef<TokenType> binOp)
		: mStartPrecedence(mPrecedenceTable.precedence(startingUnaryOperator, Arity::unary))
		, mPrimary(primary)
		, mBinOp(binOp)
	{
	}

	QSharedPointer<ast::Node> parse(TokenStream<TokenType> &tokenStream, ParserContext<TokenType> &parserContext) const override
	{
		return parse(tokenStream, parserContext, mStartPrecedence);
	}

	QSharedPointer<ast::Node> parse(TokenStream<TokenType> &tokenStream, ParserContext<TokenType> &parserContext, int currentPrecedence) const
	{
		auto resultAst = mPrimary->parse(tokenStream, parserContext);

		while (mPrecedenceTable.binaryOperators().contains(tokenStream.next().token())
				&& mPrecedenceTable.precedence(tokenStream.next().token(), Arity::binary) >= currentPrecedence)
		{
			int const newPrecedence = mPrecedenceTable.associativity(tokenStream.next().token()) == Associativity::left
					? 1 + mPrecedenceTable.precedence(tokenStream.next().token(), Arity::binary)
					: mPrecedenceTable.precedence(tokenStream.next().token(), Arity::binary)
					;

			auto binOpResult = mBinOp->parse(tokenStream, parserContext);

			auto rightOperandResult = parse(tokenStream, parserContext, newPrecedence);

			auto op = as<ast::BinaryOperator>(binOpResult);
			op->setLeftOperand(resultAst);
			op->setRightOperand(rightOperandResult);
			resultAst = op;
		}

		return resultAst;
	}


	QSet<TokenType> first() const override
	{
		return mPrimary->first();
	}

private:
	PrecedenceTable<TokenType> const mPrecedenceTable;
	int const mStartPrecedence;

	ParserRef<TokenType> const mPrimary;
	ParserRef<TokenType> const mBinOp;
};

}
}
}
