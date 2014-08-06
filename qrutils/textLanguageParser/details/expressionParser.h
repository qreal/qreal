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

	TextLanguageParserInterface::Result parse(TokenStream &tokenStream) const override
	{
		return parse(tokenStream, mStartPrecedence);
	}

	TextLanguageParserInterface::Result parse(TokenStream &tokenStream, int currentPrecedence) const
	{
		TextLanguageParserInterface::Result primaryResult = mPrimary->parse(tokenStream);
		QSharedPointer<ast::Node> resultAst = primaryResult.astRoot;
		QList<ParserError> errors = primaryResult.errors;

		while (mPrecedenceTable.binaryOperators().contains(tokenStream.next().token())
				&& mPrecedenceTable.precedence(tokenStream.next().token()) >= currentPrecedence)
		{
			int const newPrecedence = mPrecedenceTable.associativity(tokenStream.next().token()) == Associativity::left
					? 1 + mPrecedenceTable.precedence(tokenStream.next().token())
					: mPrecedenceTable.precedence(tokenStream.next().token())
					;

			TextLanguageParserInterface::Result binOpResult = mBinOp->parse(tokenStream);

			TextLanguageParserInterface::Result rightOperandResult = parse(tokenStream, newPrecedence);

			QSharedPointer<ast::BinaryOperator> op = binOpResult.astRoot.staticCast<ast::BinaryOperator>();
			op->setLeftOperand(resultAst);
			op->setRightOperand(rightOperandResult.astRoot);
			resultAst = op;

			errors << binOpResult.errors;
			errors << rightOperandResult.errors;
		}

		return TextLanguageParserInterface::Result(resultAst, errors);
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
