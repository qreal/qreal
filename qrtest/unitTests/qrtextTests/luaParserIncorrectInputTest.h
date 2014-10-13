#pragma once

#include <QtCore/QScopedPointer>

#include <gtest/gtest.h>

#include "qrtext/src/lua/luaParser.h"
#include "qrtext/src/lua/luaLexer.h"

namespace qrTest {

class LuaParserIncorrectInputTest : public testing::Test
{
protected:
	void SetUp() override;

	QScopedPointer<qrtext::lua::details::LuaParser> mParser;
	QScopedPointer<qrtext::lua::details::LuaLexer> mLexer;
	QList<qrtext::core::Error> mErrors;
};

}
