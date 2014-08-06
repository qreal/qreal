#include "textLanguageParserTest.h"

#include "textLanguageParser/ast/number.h"
#include "textLanguageParser/ast/unaryOperator.h"

#include "gtest/gtest.h"

using namespace textLanguageParser;
using namespace qrTest;

void TextLanguageParserTest::SetUp()
{
	mParser.reset(new textLanguageParser::TextLanguageParser());
}

//TEST_F(TextLanguageParserTest, sanityCheck)
//{
//	QString const stream = "123";

//	auto result = mParser->parse(stream);

//	EXPECT_TRUE(result.errors.isEmpty());
//	EXPECT_NE(nullptr, result.astRoot);

//	auto number = dynamic_cast<ast::Number *>(result.astRoot.data());

//	ASSERT_NE(nullptr, number);

//	EXPECT_EQ("123", number->stringRepresentation());
//}

TEST_F(TextLanguageParserTest, unaryOp)
{
	QString const stream = "-123";

	auto result = mParser->parse(stream);

	EXPECT_TRUE(result.errors.isEmpty());
	EXPECT_NE(nullptr, result.astRoot);

	QSharedPointer<ast::UnaryOperator> unaryOp = result.astRoot.dynamicCast<ast::UnaryOperator>();

	ASSERT_FALSE(unaryOp.isNull());

	EXPECT_EQ(ast::UnaryOperator::Type::minus, unaryOp->type());

	QSharedPointer<ast::Number> operand = unaryOp->operand().dynamicCast<ast::Number>();

	ASSERT_FALSE(operand.isNull());

	EXPECT_EQ("123", operand->stringRepresentation());
}

TEST_F(TextLanguageParserTest, connections)
{
	QString const stream = "-123";

	auto result = mParser->parse(stream);

	EXPECT_TRUE(result.errors.isEmpty());
	EXPECT_NE(nullptr, result.astRoot);

	QSharedPointer<ast::UnaryOperator> unaryOp = result.astRoot.dynamicCast<ast::UnaryOperator>();

	ASSERT_FALSE(unaryOp.isNull());

	EXPECT_EQ(ast::Connection(0, 0, 0), unaryOp->start());

	QSharedPointer<ast::Number> operand = unaryOp->operand().dynamicCast<ast::Number>();

	ASSERT_FALSE(operand.isNull());

	EXPECT_EQ(ast::Connection(1, 0, 1), operand->start());
	EXPECT_EQ(ast::Connection(3, 0, 3), operand->end());
}
