#pragma once

#include <QtCore/QScopedPointer>

#include <gtest/gtest.h>

#include "qrtext/src/lua/luaInterpreter.h"
#include "qrtext/src/lua/luaSemanticAnalyzer.h"
#include "qrtext/src/lua/luaParser.h"
#include "qrtext/src/lua/luaLexer.h"

namespace qrTest {

class LuaInterpreterIncorrectInputTest : public testing::Test
{
protected:
	void SetUp() override;

	QSharedPointer<qrtext::core::ast::Node> parseAndAnalyze(const QString &code);

	void interpret(const QString &code) {
		auto const ast = parseAndAnalyze(code);
		if (mErrors.isEmpty()) {
			mInterpreter->interpret(ast, *mAnalyzer);
		}
	}

	QScopedPointer<qrtext::lua::details::LuaInterpreter> mInterpreter;
	QScopedPointer<qrtext::lua::details::LuaSemanticAnalyzer> mAnalyzer;
	QScopedPointer<qrtext::lua::details::LuaParser> mParser;
	QScopedPointer<qrtext::lua::details::LuaLexer> mLexer;
	QList<qrtext::core::Error> mErrors;
};

}
