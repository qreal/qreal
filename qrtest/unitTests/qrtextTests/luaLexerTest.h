#pragma once

#include <QtCore/QScopedPointer>

#include <gtest/gtest.h>

#include "qrtext/src/lua/luaLexer.h"

namespace qrTest {

class LuaLexerTest : public testing::Test
{
protected:
	void SetUp() override;

	QScopedPointer<qrtext::lua::details::LuaLexer> mLexer;
	QList<qrtext::core::Error> mErrors;
};

}
