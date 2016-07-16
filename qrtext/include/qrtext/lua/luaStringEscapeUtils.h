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

#pragma once

#include <QtCore/QString>

#include "qrtext/declSpec.h"

namespace qrtext {
namespace lua {

/// Contains methods for working with escape sequences in C-like languages, which suddenly appeared in Lua parser
/// due to feature creep.
class QRTEXT_EXPORT LuaStringEscapeUtils
{
public:
	/// Replaces escape characters to their string representations (for example, "\t" will be replaced to "\\t").
	static QString escape(const QString &string);

	/// Replaces string that looks like escape character with escape character (for example, "\\t" will be replaced
	/// to "\t").
	static QString unescape(const QString &string);
};

}
}
