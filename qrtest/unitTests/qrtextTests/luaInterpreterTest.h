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

	QSharedPointer<qrtext::core::ast::Node> parseAndAnalyze(QString const &code);

	template<typename T>
	T interpret(QString const &code) {
		return mInterpreter->interpret(parseAndAnalyze(code), *mAnalyzer).value<T>();
	}

	QScopedPointer<qrtext::lua::details::LuaInterpreter> mInterpreter;
	QScopedPointer<qrtext::lua::details::LuaSemanticAnalyzer> mAnalyzer;
	QScopedPointer<qrtext::lua::details::LuaParser> mParser;
	QScopedPointer<qrtext::lua::details::LuaLexer> mLexer;
	QList<qrtext::core::Error> mErrors;
};

}
