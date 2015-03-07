#include "lexerTest.h"

using namespace qrTest;

void LexerTest::SetUp() {
	mErrors.clear();
	mLexer.reset(new simpleParser::Lexer(mErrors));
}

void LexerTest::TearDown() {
//	mErrors.clear();
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
}
