/* Copyright 2007-2015 QReal Research Group
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

#include <QtCore/QScopedPointer>

#include "qrtext/core/lexer/lexer.h"
#include "qrtext/src/lua/luaTokenTypes.h"

namespace qrtext {
namespace lua {
namespace details {

/// Lexer of something like Lua 5.3 based on regular expressions. Provides a list of tokens by given input string.
/// Allows Unicode input.
///
/// Now lexer (with default token patterns) follows Lua 5.3 specification with following exceptions:
/// - long brackets are not supported, either for string literals or for comments.
class LuaLexer: public core::Lexer<LuaTokenTypes>
{
public:
	/// Constructor.
	/// @param errors - error stream to report errors to.
	LuaLexer(QList<core::Error> &errors);

private:
	static core::TokenPatterns<LuaTokenTypes> initPatterns();
};

}
}
}
