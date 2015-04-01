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
	const QString stream =
		"'enum State {' newline \n"
			"foreach (state in State) {\n"
			"tab state.Name ',' \n"
			"newline \n"
		"} \n"
		"tab StartState.Name ',' newline \n"
		"tab EndState.Name newline \n"
		"'}' \n";

	const auto lexerResult = mLexer->tokenize(stream);
	const auto parserResult = mParser->parse(lexerResult, mLexer->userFriendlyTokenNames());

	ASSERT_EQ(parserResult->children().size(), 11);
}
