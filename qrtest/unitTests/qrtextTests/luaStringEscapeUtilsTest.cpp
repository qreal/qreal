/* Copyright 2016 CyberTech Labs Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include <QtCore/QFile>

#include <qrtext/lua/luaStringEscapeUtils.h>

#include "gtest/gtest.h"

using namespace qrtext::lua;

TEST(LuaStringEscapeUtilsTest, unescapeTest)
{
	ASSERT_EQ(QString(""), LuaStringEscapeUtils::unescape(""));
	ASSERT_EQ(QString("\t"), LuaStringEscapeUtils::unescape("\\t"));
	ASSERT_EQ(QString("\\t"), LuaStringEscapeUtils::unescape("\\\\t"));
	ASSERT_EQ(QString("a\t"), LuaStringEscapeUtils::unescape("a\\t"));
	ASSERT_EQ(QString("a\\t"), LuaStringEscapeUtils::unescape("a\\\\t"));
	ASSERT_EQ(QString("\ta"), LuaStringEscapeUtils::unescape("\\ta"));
	ASSERT_EQ(QString("\\ta"), LuaStringEscapeUtils::unescape("\\\\ta"));
	ASSERT_EQ(QString("\t\t"), LuaStringEscapeUtils::unescape("\\t\\t"));
	ASSERT_EQ(QString("\\t\t"), LuaStringEscapeUtils::unescape("\\\\t\\t"));

	ASSERT_EQ(QString("\\"), LuaStringEscapeUtils::unescape("\\\\"));
	ASSERT_EQ(QString("\'"), LuaStringEscapeUtils::unescape("\\'"));
	ASSERT_EQ(QString("\""), LuaStringEscapeUtils::unescape("\\\""));
	ASSERT_EQ(QString("\?"), LuaStringEscapeUtils::unescape("\\?"));
	ASSERT_EQ(QString("\a"), LuaStringEscapeUtils::unescape("\\a"));
	ASSERT_EQ(QString("\b"), LuaStringEscapeUtils::unescape("\\b"));
	ASSERT_EQ(QString("\f"), LuaStringEscapeUtils::unescape("\\f"));
	ASSERT_EQ(QString("\n"), LuaStringEscapeUtils::unescape("\\n"));
	ASSERT_EQ(QString("\r"), LuaStringEscapeUtils::unescape("\\r"));
	ASSERT_EQ(QString("\t"), LuaStringEscapeUtils::unescape("\\t"));
	ASSERT_EQ(QString("\v"), LuaStringEscapeUtils::unescape("\\v"));
}

TEST(LuaStringEscapeUtilsTest, escapeTest)
{
	ASSERT_EQ(QString(""), LuaStringEscapeUtils::escape(""));
	ASSERT_EQ(QString("\\t"), LuaStringEscapeUtils::escape("\t"));
	ASSERT_EQ(QString("\\\\t"), LuaStringEscapeUtils::escape("\\t"));
	ASSERT_EQ(QString("\\\\\\t"), LuaStringEscapeUtils::escape("\\\t"));

	ASSERT_EQ(QString("a\\t"), LuaStringEscapeUtils::escape("a\t"));
	ASSERT_EQ(QString("a\\\\t"), LuaStringEscapeUtils::escape("a\\t"));
	ASSERT_EQ(QString("\\ta"), LuaStringEscapeUtils::escape("\ta"));
	ASSERT_EQ(QString("\\\\ta"), LuaStringEscapeUtils::escape("\\ta"));
	ASSERT_EQ(QString("\\t\\t"), LuaStringEscapeUtils::escape("\t\t"));
	ASSERT_EQ(QString("\\\\t\\t"), LuaStringEscapeUtils::escape("\\t\t"));

	ASSERT_EQ(QString("\\\\"), LuaStringEscapeUtils::escape("\\"));
	ASSERT_EQ(QString("\\\'"), LuaStringEscapeUtils::escape("\'"));
	ASSERT_EQ(QString("\\\""), LuaStringEscapeUtils::escape("\""));
	ASSERT_EQ(QString("\\?"), LuaStringEscapeUtils::escape("\?"));
	ASSERT_EQ(QString("\\a"), LuaStringEscapeUtils::escape("\a"));
	ASSERT_EQ(QString("\\b"), LuaStringEscapeUtils::escape("\b"));
	ASSERT_EQ(QString("\\f"), LuaStringEscapeUtils::escape("\f"));
	ASSERT_EQ(QString("\\n"), LuaStringEscapeUtils::escape("\n"));
	ASSERT_EQ(QString("\\r"), LuaStringEscapeUtils::escape("\r"));
	ASSERT_EQ(QString("\\t"), LuaStringEscapeUtils::escape("\t"));
	ASSERT_EQ(QString("\\v"), LuaStringEscapeUtils::escape("\v"));
}
