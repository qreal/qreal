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

#include "qrtext/lua/luaStringEscapeUtils.h"

using namespace qrtext;
using namespace lua;

QString LuaStringEscapeUtils::escape(const QString &string)
{
	auto transformedString = string;
	for (int i = 0; i < transformedString.length(); ++i) {
		const auto replace = [&](const QString &str) {
			transformedString.replace(i, 1, str);
			++i;
		};

		switch (transformedString[i].toLatin1()) {
		case '\\':
			replace("\\\\");
			break;
		case '\'':
			replace("\\'");
			break;
		case '"':
			replace("\\\"");
			break;
		case '\?':
			replace("\\?");
			break;
		case '\a':
			replace("\\a");
			break;
		case '\b':
			replace("\\b");
			break;
		case '\f':
			replace("\\f");
			break;
		case '\n':
			replace("\\n");
			break;
		case '\r':
			replace("\\r");
			break;
		case '\t':
			replace("\\t");
			break;
		case '\v':
			replace("\\v");
			break;
		default:
			break;
		}
	}

	return transformedString;
}

QString LuaStringEscapeUtils::unescape(const QString &string)
{
	auto transformedString = string;
	for (int i = 0; i < transformedString.length() - 1; ++i) {
		if (transformedString[i] == '\\') {
			const auto replace = [&](QChar c) {
				transformedString.replace(i, 2, c);
			};

			switch (transformedString[i + 1].toLatin1()) {
			case '\\':
				replace('\\');
				break;
			case '\'':
				replace('\'');
				break;
			case '"':
				replace('"');
				break;
			case '?':
				replace('\?');
				break;
			case 'a':
				replace('\a');
				break;
			case 'b':
				replace('\b');
				break;
			case 'f':
				replace('\f');
				break;
			case 'n':
				replace('\n');
				break;
			case 'r':
				replace('\r');
				break;
			case 't':
				replace('\t');
				break;
			case 'v':
				replace('\v');
				break;
			default:
				break;
			}
		}
	}

	return transformedString;
}
