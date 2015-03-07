#include "lexerTest.h"

#include <plugins/generationRulesTool/tokenTypes.h>

using namespace qrTest;

void LexerTest::SetUp() {
	mErrors.clear();
	mLexer.reset(new simpleParser::Lexer(mErrors));
}

void LexerTest::TearDown() {
	mErrors.clear();
}

TEST_F(LexerTest, lexerTestForForeachExample) {
	QString stream = "'enum State {'\n"
				"foreach (State) {\n"
				"State.name ',' \n"
				"newline \n"
				"} \n"
				"StartState.name ',' newline \n"
				"EndState.name newline \n"
				"'}' \n";

	auto lexerResult = mLexer->tokenize(stream);

	ASSERT_EQ(lexerResult.length(), 22);

	EXPECT_EQ(simpleParser::TokenTypes::text, lexerResult[0].token());
	EXPECT_EQ(simpleParser::TokenTypes::foreachKeyword, lexerResult[1].token());
	EXPECT_EQ(simpleParser::TokenTypes::openingBracket, lexerResult[2].token());
	EXPECT_EQ(simpleParser::TokenTypes::identifier, lexerResult[3].token());
	EXPECT_EQ(simpleParser::TokenTypes::closingBracket, lexerResult[4].token());

	EXPECT_EQ(simpleParser::TokenTypes::openingCurlyBracket, lexerResult[5].token());
	EXPECT_EQ(simpleParser::TokenTypes::identifier, lexerResult[6].token());
	EXPECT_EQ(simpleParser::TokenTypes::dot, lexerResult[7].token());
	EXPECT_EQ(simpleParser::TokenTypes::identifier, lexerResult[8].token());
	EXPECT_EQ(simpleParser::TokenTypes::text, lexerResult[9].token());

	EXPECT_EQ(simpleParser::TokenTypes::newlineKeyword, lexerResult[10].token());
	EXPECT_EQ(simpleParser::TokenTypes::closingCurlyBracket, lexerResult[11].token());
	EXPECT_EQ(simpleParser::TokenTypes::identifier, lexerResult[12].token());
	EXPECT_EQ(simpleParser::TokenTypes::dot, lexerResult[13].token());
	EXPECT_EQ(simpleParser::TokenTypes::identifier, lexerResult[14].token());

	EXPECT_EQ(simpleParser::TokenTypes::text, lexerResult[15].token());
	EXPECT_EQ(simpleParser::TokenTypes::newlineKeyword, lexerResult[16].token());
	EXPECT_EQ(simpleParser::TokenTypes::identifier, lexerResult[17].token());
	EXPECT_EQ(simpleParser::TokenTypes::dot, lexerResult[18].token());
	EXPECT_EQ(simpleParser::TokenTypes::identifier, lexerResult[19].token());

	EXPECT_EQ(simpleParser::TokenTypes::newlineKeyword, lexerResult[20].token());
	EXPECT_EQ(simpleParser::TokenTypes::text, lexerResult[21].token());
}
