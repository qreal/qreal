#include "parserTest.h"

using namespace qrTest;

void ParserTest::SetUp()
{
	mParserErrors.clear();
	mLexerErrors.clear();

	mLexer.reset(new simpleParser::Lexer(mLexerErrors));
	mParser.reset(new simpleParser::Parser(mParserErrors));
}

void ParserTest::TearDown()
{
	mParserErrors.clear();
	mLexerErrors.clear();
}

TEST_F(ParserTest, parserTestForForeachExample)
{
	const QString stream = "'enum State {'\n"
				"foreach (State) {\n"
				"State.name ',' \n"
				"newline \n"
				"} \n"
				"StartState.name ',' newline \n"
				"EndState.name newline \n"
				"'}' \n";

	const auto lexerResult = mLexer->tokenize(stream);
	const auto parserResult = mParser->parse(lexerResult, mLexer->userFriendlyTokenNames());

	ASSERT_EQ(parserResult->children().size(), 8);
}
