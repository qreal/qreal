#pragma once

#include <QtCore/QScopedPointer>

#include <gtest/gtest.h>

#include "qrtext/src/lua/luaInterpreter.h"
#include "qrtext/src/lua/luaSemanticAnalyzer.h"
#include "qrtext/src/lua/luaParser.h"
#include "qrtext/src/lua/luaLexer.h"

namespace qrTest {

class LuaInterpreterTest : public testing::Test
{
protected:
	void SetUp() override;

	QSharedPointer<qrtext::core::ast::Node> parseAndAnalyze(const QString &code);

	template<typename T>
	T interpret(const QString &code) {
		auto const ast = parseAndAnalyze(code);
		if (mErrors.isEmpty()) {
			return mInterpreter->interpret(ast, *mAnalyzer).value<T>();
		} else {
			return {};
		}
	}

	QScopedPointer<qrtext::lua::details::LuaInterpreter> mInterpreter;
	QScopedPointer<qrtext::lua::details::LuaSemanticAnalyzer> mAnalyzer;
	QScopedPointer<qrtext::lua::details::LuaParser> mParser;
	QScopedPointer<qrtext::lua::details::LuaLexer> mLexer;
	QList<qrtext::core::Error> mErrors;
};

}
