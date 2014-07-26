#include "lexerTest.h"

#include "gtest/gtest.h"

using namespace textLanguageParser;
using namespace qrTest;

void LexerTest::SetUp()
{
	mLexer.reset(new textLanguageParser::details::Lexer(mLexemes));
}

TEST_F(LexerTest, tokenizeSanityCheck)
{
	QString const stream = "  123 abc";

	auto result = mLexer->tokenize(stream);

	ASSERT_TRUE(result.comments.isEmpty());
	ASSERT_TRUE(result.errors.isEmpty());
	ASSERT_EQ(2, result.tokens.size());
	ASSERT_EQ(Lexemes::integer, result.tokens[0].lexeme());
	ASSERT_EQ(Lexemes::identifier, result.tokens[1].lexeme());
}
