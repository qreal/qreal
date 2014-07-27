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
	EXPECT_EQ(Lexemes::number, result.tokens[0].lexeme());
	EXPECT_EQ(Lexemes::identifier, result.tokens[1].lexeme());
}

TEST_F(LexerTest, connections)
{
	QString const stream = "  123 abc\n  456";

	auto result = mLexer->tokenize(stream);

	ASSERT_EQ(3, result.tokens.size());

	EXPECT_EQ(ast::Connection(2, 0, 2), result.tokens[0].range().start());
	EXPECT_EQ(ast::Connection(4, 0, 4), result.tokens[0].range().end());

	EXPECT_EQ(ast::Connection(6, 0, 6), result.tokens[1].range().start());
	EXPECT_EQ(ast::Connection(8, 0, 8), result.tokens[1].range().end());

	EXPECT_EQ(ast::Connection(12, 1, 2), result.tokens[2].range().start());
	EXPECT_EQ(ast::Connection(14, 1, 4), result.tokens[2].range().end());
}

TEST_F(LexerTest, customization)
{
	mLexemes->redefine(Lexemes::number, QRegularExpression("[+-]?\\d+x"));

	QString const stream = "+12x 45x";

	auto result = mLexer->tokenize(stream);

	ASSERT_EQ(2, result.tokens.size());

	EXPECT_EQ(Lexemes::number, result.tokens[0].lexeme());
	EXPECT_EQ(0, result.tokens[0].range().start().absolutePosition());
	EXPECT_EQ(3, result.tokens[0].range().end().absolutePosition());

	EXPECT_EQ(Lexemes::number, result.tokens[1].lexeme());
	EXPECT_EQ(5, result.tokens[1].range().start().absolutePosition());
	EXPECT_EQ(7, result.tokens[1].range().end().absolutePosition());
}

TEST_F(LexerTest, keywords)
{
	QString const stream = "and And _and +break";

	auto result = mLexer->tokenize(stream);

	ASSERT_EQ(5, result.tokens.size());

	EXPECT_EQ(Lexemes::andKeyword, result.tokens[0].lexeme());
	EXPECT_EQ(Lexemes::identifier, result.tokens[1].lexeme());
	EXPECT_EQ(Lexemes::identifier, result.tokens[2].lexeme());
	EXPECT_EQ(Lexemes::plus, result.tokens[3].lexeme());
	EXPECT_EQ(Lexemes::breakKeyword, result.tokens[4].lexeme());
}

TEST_F(LexerTest, lexemeTypes)
{
	QString const stream = "ololo and break do else elseif end false for function goto if in local nil not or repeat"
			" return then true until while + - * / % ^ # == ~= <= >= < > = ( ) { } [ ] :: ; : , . .. ...";

	auto result = mLexer->tokenize(stream);

	ASSERT_EQ(50, result.tokens.size());

	EXPECT_EQ(Lexemes::identifier, result.tokens[0].lexeme());

	EXPECT_EQ(Lexemes::andKeyword, result.tokens[1].lexeme());
	EXPECT_EQ(Lexemes::breakKeyword, result.tokens[2].lexeme());
	EXPECT_EQ(Lexemes::doKeyword, result.tokens[3].lexeme());
	EXPECT_EQ(Lexemes::elseKeyword, result.tokens[4].lexeme());
	EXPECT_EQ(Lexemes::elseifKeyword, result.tokens[5].lexeme());
	EXPECT_EQ(Lexemes::endKeyword, result.tokens[6].lexeme());
	EXPECT_EQ(Lexemes::falseKeyword, result.tokens[7].lexeme());
	EXPECT_EQ(Lexemes::forKeyword, result.tokens[8].lexeme());
	EXPECT_EQ(Lexemes::functionKeyword, result.tokens[9].lexeme());
	EXPECT_EQ(Lexemes::gotoKeyword, result.tokens[10].lexeme());
	EXPECT_EQ(Lexemes::ifKeyword, result.tokens[11].lexeme());
	EXPECT_EQ(Lexemes::inKeyword, result.tokens[12].lexeme());
	EXPECT_EQ(Lexemes::localKeyword, result.tokens[13].lexeme());
	EXPECT_EQ(Lexemes::nilKeyword, result.tokens[14].lexeme());
	EXPECT_EQ(Lexemes::notKeyword, result.tokens[15].lexeme());
	EXPECT_EQ(Lexemes::orKeyword, result.tokens[16].lexeme());
	EXPECT_EQ(Lexemes::repeatKeyword, result.tokens[17].lexeme());
	EXPECT_EQ(Lexemes::returnKeyword, result.tokens[18].lexeme());
	EXPECT_EQ(Lexemes::thenKeyword, result.tokens[19].lexeme());
	EXPECT_EQ(Lexemes::trueKeyword, result.tokens[20].lexeme());
	EXPECT_EQ(Lexemes::untilKeyword, result.tokens[21].lexeme());
	EXPECT_EQ(Lexemes::whileKeyword, result.tokens[22].lexeme());

	EXPECT_EQ(Lexemes::plus, result.tokens[23].lexeme());
	EXPECT_EQ(Lexemes::minus, result.tokens[24].lexeme());
	EXPECT_EQ(Lexemes::asterick, result.tokens[25].lexeme());
	EXPECT_EQ(Lexemes::slash, result.tokens[26].lexeme());
	EXPECT_EQ(Lexemes::percent, result.tokens[27].lexeme());
	EXPECT_EQ(Lexemes::hat, result.tokens[28].lexeme());
	EXPECT_EQ(Lexemes::sharp, result.tokens[29].lexeme());
	EXPECT_EQ(Lexemes::doubleEquals, result.tokens[30].lexeme());
	EXPECT_EQ(Lexemes::tildaEquals, result.tokens[31].lexeme());
	EXPECT_EQ(Lexemes::lessEquals, result.tokens[32].lexeme());
	EXPECT_EQ(Lexemes::greaterEquals, result.tokens[33].lexeme());
	EXPECT_EQ(Lexemes::less, result.tokens[34].lexeme());
	EXPECT_EQ(Lexemes::greater, result.tokens[35].lexeme());
	EXPECT_EQ(Lexemes::equals, result.tokens[36].lexeme());
	EXPECT_EQ(Lexemes::openingBracket, result.tokens[37].lexeme());
	EXPECT_EQ(Lexemes::closingBracket, result.tokens[38].lexeme());
	EXPECT_EQ(Lexemes::openingCurlyBracket, result.tokens[39].lexeme());
	EXPECT_EQ(Lexemes::closingCurlyBracket, result.tokens[40].lexeme());
	EXPECT_EQ(Lexemes::openingSquareBracket, result.tokens[41].lexeme());
	EXPECT_EQ(Lexemes::closingSquareBracket, result.tokens[42].lexeme());
	EXPECT_EQ(Lexemes::doubleColon, result.tokens[43].lexeme());
	EXPECT_EQ(Lexemes::semicolon, result.tokens[44].lexeme());
	EXPECT_EQ(Lexemes::colon, result.tokens[45].lexeme());
	EXPECT_EQ(Lexemes::comma, result.tokens[46].lexeme());
	EXPECT_EQ(Lexemes::dot, result.tokens[47].lexeme());
	EXPECT_EQ(Lexemes::doubleDot, result.tokens[48].lexeme());
	EXPECT_EQ(Lexemes::tripleDot, result.tokens[49].lexeme());
}

TEST_F(LexerTest, errorReporting)
{
	QString const stream = "ololo ~~= !!! nil";

	auto result = mLexer->tokenize(stream);

	ASSERT_EQ(2, result.tokens.size());
	ASSERT_EQ(2, result.errors.size());

	EXPECT_EQ(Lexemes::identifier, result.tokens[0].lexeme());
	EXPECT_EQ(Lexemes::nilKeyword, result.tokens[1].lexeme());

	EXPECT_EQ(QString("Lexer error"), result.errors[0].errorMessage());
	EXPECT_EQ(ast::Connection(6, 0, 6), result.errors[0].connection());
	EXPECT_EQ(ParserError::ErrorType::lexicalError, result.errors[0].errorType());
	EXPECT_EQ(ParserError::Severity::error, result.errors[0].severity());

	EXPECT_EQ(QString("Lexer error"), result.errors[1].errorMessage());
	EXPECT_EQ(ast::Connection(10, 0, 10), result.errors[1].connection());
	EXPECT_EQ(ParserError::ErrorType::lexicalError, result.errors[1].errorType());
	EXPECT_EQ(ParserError::Severity::error, result.errors[1].severity());

	EXPECT_EQ(14, result.tokens[1].range().start().absolutePosition());
}

TEST_F(LexerTest, multilineErrorReporting)
{
	QString const stream = "ololo ~~=\n !!! nil";

	auto result = mLexer->tokenize(stream);

	ASSERT_EQ(2, result.tokens.size());
	ASSERT_EQ(2, result.errors.size());

	EXPECT_EQ(Lexemes::identifier, result.tokens[0].lexeme());
	EXPECT_EQ(Lexemes::nilKeyword, result.tokens[1].lexeme());

	EXPECT_EQ(QString("Lexer error"), result.errors[0].errorMessage());
	EXPECT_EQ(ast::Connection(6, 0, 6), result.errors[0].connection());
	EXPECT_EQ(ParserError::ErrorType::lexicalError, result.errors[0].errorType());
	EXPECT_EQ(ParserError::Severity::error, result.errors[0].severity());

	EXPECT_EQ(QString("Lexer error"), result.errors[1].errorMessage());
	EXPECT_EQ(ast::Connection(11, 1, 1), result.errors[1].connection());
	EXPECT_EQ(ParserError::ErrorType::lexicalError, result.errors[1].errorType());
	EXPECT_EQ(ParserError::Severity::error, result.errors[1].severity());

	EXPECT_EQ(15, result.tokens[1].range().start().absolutePosition());
}
