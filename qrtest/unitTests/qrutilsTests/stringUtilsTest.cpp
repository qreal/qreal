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

#include <qrutils/stringUtils.h>

#include "gtest/gtest.h"

using namespace utils;

TEST(StringUtilsTest, unescapeTest)
{
	ASSERT_EQ(QString(""), StringUtils::unescape(""));
	ASSERT_EQ(QString("\t"), StringUtils::unescape("\\t"));
	ASSERT_EQ(QString("\\t"), StringUtils::unescape("\\\\t"));
	ASSERT_EQ(QString("a\t"), StringUtils::unescape("a\\t"));
	ASSERT_EQ(QString("a\\t"), StringUtils::unescape("a\\\\t"));
	ASSERT_EQ(QString("\ta"), StringUtils::unescape("\\ta"));
	ASSERT_EQ(QString("\\ta"), StringUtils::unescape("\\\\ta"));
	ASSERT_EQ(QString("\t\t"), StringUtils::unescape("\\t\\t"));
	ASSERT_EQ(QString("\\t\t"), StringUtils::unescape("\\\\t\\t"));

	ASSERT_EQ(QString("\\"), StringUtils::unescape("\\\\"));
	ASSERT_EQ(QString("\'"), StringUtils::unescape("\\'"));
	ASSERT_EQ(QString("\""), StringUtils::unescape("\\\""));
	ASSERT_EQ(QString("\?"), StringUtils::unescape("\\?"));
	ASSERT_EQ(QString("\a"), StringUtils::unescape("\\a"));
	ASSERT_EQ(QString("\b"), StringUtils::unescape("\\b"));
	ASSERT_EQ(QString("\f"), StringUtils::unescape("\\f"));
	ASSERT_EQ(QString("\n"), StringUtils::unescape("\\n"));
	ASSERT_EQ(QString("\r"), StringUtils::unescape("\\r"));
	ASSERT_EQ(QString("\t"), StringUtils::unescape("\\t"));
	ASSERT_EQ(QString("\v"), StringUtils::unescape("\\v"));
}

TEST(StringUtilsTest, escapeTest)
{
	ASSERT_EQ(QString(""), StringUtils::escape(""));
	ASSERT_EQ(QString("\\t"), StringUtils::escape("\t"));
	ASSERT_EQ(QString("\\\\t"), StringUtils::escape("\\t"));
	ASSERT_EQ(QString("\\\\\\t"), StringUtils::escape("\\\t"));

	ASSERT_EQ(QString("a\\t"), StringUtils::escape("a\t"));
	ASSERT_EQ(QString("a\\\\t"), StringUtils::escape("a\\t"));
	ASSERT_EQ(QString("\\ta"), StringUtils::escape("\ta"));
	ASSERT_EQ(QString("\\\\ta"), StringUtils::escape("\\ta"));
	ASSERT_EQ(QString("\\t\\t"), StringUtils::escape("\t\t"));
	ASSERT_EQ(QString("\\\\t\\t"), StringUtils::escape("\\t\t"));

	ASSERT_EQ(QString("\\\\"), StringUtils::escape("\\"));
	ASSERT_EQ(QString("\\\'"), StringUtils::escape("\'"));
	ASSERT_EQ(QString("\\\""), StringUtils::escape("\""));
	ASSERT_EQ(QString("\\?"), StringUtils::escape("\?"));
	ASSERT_EQ(QString("\\a"), StringUtils::escape("\a"));
	ASSERT_EQ(QString("\\b"), StringUtils::escape("\b"));
	ASSERT_EQ(QString("\\f"), StringUtils::escape("\f"));
	ASSERT_EQ(QString("\\n"), StringUtils::escape("\n"));
	ASSERT_EQ(QString("\\r"), StringUtils::escape("\r"));
	ASSERT_EQ(QString("\\t"), StringUtils::escape("\t"));
	ASSERT_EQ(QString("\\v"), StringUtils::escape("\v"));
}
