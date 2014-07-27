#include "lexerTest.h"

#include "gtest/gtest.h"

using namespace textLanguageParser;
using namespace qrTest;

void LexerTest::SetUp()
{
	mLexemes.reset(new Lexemes());
	mLexer.reset(new textLanguageParser::details::Lexer(*mLexemes));
}

TEST_F(LexerTest, tokenizeSanityCheck)
{
	QString const stream = "  123 abc";

	auto result = mLexer->tokenize(stream);

	EXPECT_TRUE(result.comments.isEmpty());
	EXPECT_TRUE(result.errors.isEmpty());
	ASSERT_EQ(2, result.tokens.size());
	EXPECT_EQ(Lexemes::integer, result.tokens[0].lexeme());
	EXPECT_EQ(Lexemes::identifier, result.tokens[1].lexeme());
}

TEST_F(LexerTest, connections)
{
	QString const stream = "  123 abc\n  456";

	auto result = mLexer->tokenize(stream);

	ASSERT_EQ(3, result.tokens.size());

	EXPECT_EQ(2, result.tokens[0].range().start().absolutePosition());
	EXPECT_EQ(4, result.tokens[0].range().end().absolutePosition());

	EXPECT_EQ(0, result.tokens[0].range().start().line());
	EXPECT_EQ(2, result.tokens[0].range().start().column());
	EXPECT_EQ(0, result.tokens[0].range().end().line());
	EXPECT_EQ(4, result.tokens[0].range().end().column());

	EXPECT_EQ(6, result.tokens[1].range().start().absolutePosition());
	EXPECT_EQ(8, result.tokens[1].range().end().absolutePosition());

	EXPECT_EQ(12, result.tokens[2].range().start().absolutePosition());
	EXPECT_EQ(14, result.tokens[2].range().end().absolutePosition());

	EXPECT_EQ(1, result.tokens[2].range().start().line());
	EXPECT_EQ(2, result.tokens[2].range().start().column());
	EXPECT_EQ(1, result.tokens[2].range().end().line());
	EXPECT_EQ(4, result.tokens[2].range().end().column());
}

TEST_F(LexerTest, customization)
{
	mLexemes->redefine(Lexemes::integer, QRegularExpression("[+-]?\\d+"));

	QString const stream = "+123 456";

	auto result = mLexer->tokenize(stream);

	ASSERT_EQ(2, result.tokens.size());

	EXPECT_EQ(Lexemes::integer, result.tokens[0].lexeme());
	EXPECT_EQ(0, result.tokens[0].range().start().absolutePosition());
	EXPECT_EQ(3, result.tokens[0].range().end().absolutePosition());

	EXPECT_EQ(Lexemes::integer, result.tokens[1].lexeme());
	EXPECT_EQ(5, result.tokens[1].range().start().absolutePosition());
	EXPECT_EQ(7, result.tokens[1].range().end().absolutePosition());
}
