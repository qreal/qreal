#pragma once

#include <QtCore/QScopedPointer>

#include <gtest/gtest.h>

#include "textLanguageParser/details/lexer.h"

namespace qrTest {

class LexerTest : public testing::Test
{
protected:
	void SetUp() override;

	QScopedPointer<textLanguageParser::details::Lexer> mLexer;
	QScopedPointer<textLanguageParser::Lexemes> mLexemes;
};

}
