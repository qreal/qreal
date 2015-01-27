#pragma once

#include <QtCore/QScopedPointer>

#include <gtest/gtest.h>

#include "qrtext/src/lua/luaSemanticAnalyzer.h"
#include "qrtext/src/lua/luaParser.h"
#include "qrtext/src/lua/luaLexer.h"

namespace qrTest {

class LuaSemanticAnalyzerTest : public testing::Test
{
protected:
	void SetUp() override;

	QSharedPointer<qrtext::core::ast::Node> parse(const QString &code);

	QScopedPointer<qrtext::lua::details::LuaSemanticAnalyzer> mAnalyzer;
	QScopedPointer<qrtext::lua::details::LuaParser> mParser;
	QScopedPointer<qrtext::lua::details::LuaLexer> mLexer;
	QList<qrtext::core::Error> mErrors;
};

}
