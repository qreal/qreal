#pragma once

#include "textLanguageParser/details/parserInterface.h"
#include "textLanguageParser/details/parserRef.h"
#include "textLanguageParser/tokenType.h"
#include "textLanguageParser/ast/binaryOperator.h"
#include "precedenceTable.h"

namespace textLanguageParser {
namespace details {

class ExpressionParser : public ParserInterface {
public:
	ExpressionParser(bool isUnary, ParserRef primary, ParserRef binOp)
		: mStartPrecedence(isUnary ? mPrecedenceTable.unaryOperatorsPrecedence() : 0)
		, mPrimary(primary)
		, mBinOp(binOp)
	{
	}

	QSharedPointer<ast::Node> parse(TokenStream &tokenStream, ParserContext &parserContext) const override
	{
		return parse(tokenStream, parserContext, mStartPrecedence);
	}

	QSharedPointer<ast::Node> parse(TokenStream &tokenStream, ParserContext &parserContext, int currentPrecedence) const
	{
		auto resultAst = mPrimary->parse(tokenStream, parserContext);

		while (mPrecedenceTable.binaryOperators().contains(tokenStream.next().token())
				&& mPrecedenceTable.precedence(tokenStream.next().token()) >= currentPrecedence)
		{
			int const newPrecedence = mPrecedenceTable.associativity(tokenStream.next().token()) == Associativity::left
					? 1 + mPrecedenceTable.precedence(tokenStream.next().token())
					: mPrecedenceTable.precedence(tokenStream.next().token())
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
	PrecedenceTable const mPrecedenceTable;
	int const mStartPrecedence;

	ParserRef const mPrimary;
	ParserRef const mBinOp;
};

}
}
