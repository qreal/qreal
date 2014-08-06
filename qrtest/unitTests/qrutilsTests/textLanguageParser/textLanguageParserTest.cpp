#include "textLanguageParserTest.h"

#include "textLanguageParser/ast/number.h"
#include "textLanguageParser/ast/unaryOperator.h"
#include "textLanguageParser/ast/binaryOperator.h"

#include "textLanguageParser/ast/addition.h"
#include "textLanguageParser/ast/multiplication.h"
#include "textLanguageParser/ast/exponentiation.h"

#include "gtest/gtest.h"

using namespace textLanguageParser;
using namespace qrTest;

void TextLanguageParserTest::SetUp()
{
	mParser.reset(new textLanguageParser::TextLanguageParser());
}

TEST_F(TextLanguageParserTest, sanityCheck)
{
	QString const stream = "123";
	auto result = mParser->parse(stream);
	EXPECT_TRUE(result.errors.isEmpty());
	auto number = result.astRoot.dynamicCast<ast::Number>();
	ASSERT_FALSE(number.isNull());
	EXPECT_EQ("123", number->stringRepresentation());
}

TEST_F(TextLanguageParserTest, unaryOp)
{
	QString const stream = "-123";
	auto result = mParser->parse(stream);
	EXPECT_TRUE(result.errors.isEmpty());
	QSharedPointer<ast::UnaryOperator> unaryOp = result.astRoot.dynamicCast<ast::UnaryOperator>();
	ASSERT_FALSE(unaryOp.isNull());
	QSharedPointer<ast::Number> operand = unaryOp->operand().dynamicCast<ast::Number>();
	ASSERT_FALSE(operand.isNull());
	EXPECT_EQ("123", operand->stringRepresentation());
}

TEST_F(TextLanguageParserTest, connections)
{
	QString const stream = "-123";
	auto result = mParser->parse(stream);
	EXPECT_TRUE(result.errors.isEmpty());
	QSharedPointer<ast::UnaryOperator> unaryOp = result.astRoot.dynamicCast<ast::UnaryOperator>();
	ASSERT_FALSE(unaryOp.isNull());
	EXPECT_EQ(ast::Connection(0, 0, 0), unaryOp->start());
	QSharedPointer<ast::Number> operand = unaryOp->operand().dynamicCast<ast::Number>();
	ASSERT_FALSE(operand.isNull());
	EXPECT_EQ(ast::Connection(1, 0, 1), operand->start());
	EXPECT_EQ(ast::Connection(3, 0, 3), operand->end());
}

TEST_F(TextLanguageParserTest, binaryOp)
{
	QString const stream = "1+2";
	auto result = mParser->parse(stream);
	EXPECT_TRUE(result.errors.isEmpty());
	QSharedPointer<ast::BinaryOperator> binaryOp = result.astRoot.dynamicCast<ast::BinaryOperator>();
	ASSERT_FALSE(binaryOp.isNull());
	QSharedPointer<ast::Number> leftOperand = binaryOp->leftOperand().dynamicCast<ast::Number>();
	ASSERT_FALSE(leftOperand.isNull());
	EXPECT_EQ("1", leftOperand->stringRepresentation());
	QSharedPointer<ast::Number> rightOperand = binaryOp->rightOperand().dynamicCast<ast::Number>();
	ASSERT_FALSE(rightOperand.isNull());
	EXPECT_EQ("2", rightOperand->stringRepresentation());
}

TEST_F(TextLanguageParserTest, associativity)
{
	// Left-associative addition.
	QString stream = "1 + 2 + 3";
	auto result = mParser->parse(stream);
	EXPECT_TRUE(result.errors.isEmpty());

	auto firstAddition = result.astRoot.dynamicCast<ast::Addition>();
	ASSERT_FALSE(firstAddition.isNull());

	auto secondAddition = firstAddition->leftOperand().dynamicCast<ast::Addition>();
	ASSERT_FALSE(secondAddition.isNull());

	auto one = secondAddition->leftOperand().dynamicCast<ast::Number>();
	ASSERT_FALSE(one.isNull());
	EXPECT_EQ("1", one->stringRepresentation());

	auto two = secondAddition->rightOperand().dynamicCast<ast::Number>();
	ASSERT_FALSE(two.isNull());
	EXPECT_EQ("2", two->stringRepresentation());

	auto three = firstAddition->rightOperand().dynamicCast<ast::Number>();
	ASSERT_FALSE(three.isNull());
	EXPECT_EQ("3", three->stringRepresentation());

	// Right-associative exponentiation.
	stream = "1 ^ 2 ^ 3";
	result = mParser->parse(stream);
	EXPECT_TRUE(result.errors.isEmpty());

	auto firstExponentiation = result.astRoot.dynamicCast<ast::Exponentiation>();
	ASSERT_FALSE(firstExponentiation.isNull());

	one = firstExponentiation->leftOperand().dynamicCast<ast::Number>();
	ASSERT_FALSE(one.isNull());
	EXPECT_EQ("1", one->stringRepresentation());

	auto secondExponentiation = firstExponentiation->rightOperand().dynamicCast<ast::Exponentiation>();
	ASSERT_FALSE(secondExponentiation.isNull());

	two = secondExponentiation->leftOperand().dynamicCast<ast::Number>();
	ASSERT_FALSE(two.isNull());
	EXPECT_EQ("2", two->stringRepresentation());

	three = secondExponentiation->rightOperand().dynamicCast<ast::Number>();
	ASSERT_FALSE(three.isNull());
	EXPECT_EQ("3", three->stringRepresentation());
}

TEST_F(TextLanguageParserTest, precedence)
{
	QString const stream = "1 * 2 + 3 * 4 + 5 * 6";
	auto result = mParser->parse(stream);
	EXPECT_TRUE(result.errors.isEmpty());

	auto firstAddition = result.astRoot.dynamicCast<ast::Addition>();
	ASSERT_FALSE(firstAddition.isNull());

	auto secondAddition = firstAddition->leftOperand().dynamicCast<ast::Addition>();
	ASSERT_FALSE(secondAddition.isNull());

	auto firstMultiplication = secondAddition->leftOperand().dynamicCast<ast::Multiplication>();
	ASSERT_FALSE(firstMultiplication.isNull());

	auto one = firstMultiplication->leftOperand().dynamicCast<ast::Number>();
	ASSERT_FALSE(one.isNull());
	EXPECT_EQ("1", one->stringRepresentation());

	auto two = firstMultiplication->rightOperand().dynamicCast<ast::Number>();
	ASSERT_FALSE(two.isNull());
	EXPECT_EQ("2", two->stringRepresentation());

	auto secondMultiplication = secondAddition->rightOperand().dynamicCast<ast::Multiplication>();
	ASSERT_FALSE(secondMultiplication.isNull());

	auto three = secondMultiplication->leftOperand().dynamicCast<ast::Number>();
	ASSERT_FALSE(three.isNull());
	EXPECT_EQ("3", three->stringRepresentation());

	auto four = secondMultiplication->rightOperand().dynamicCast<ast::Number>();
	ASSERT_FALSE(four.isNull());
	EXPECT_EQ("4", four->stringRepresentation());

	auto thirdMultiplication = firstAddition->rightOperand().dynamicCast<ast::Multiplication>();
	ASSERT_FALSE(thirdMultiplication.isNull());

	auto five = thirdMultiplication->leftOperand().dynamicCast<ast::Number>();
	ASSERT_FALSE(five.isNull());
	EXPECT_EQ("5", five->stringRepresentation());

	auto six = thirdMultiplication->rightOperand().dynamicCast<ast::Number>();
	ASSERT_FALSE(six.isNull());
	EXPECT_EQ("6", six->stringRepresentation());
}
