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

#include "luaPrecedenceTable.h"

using namespace qrtext::lua::details;
using namespace qrtext::core;

LuaPrecedenceTable::LuaPrecedenceTable()
{
	addOperator(LuaTokenTypes::orKeyword, 0, Associativity::left);
	addOperator(LuaTokenTypes::doubleVerticalLine, 0, Associativity::left);

	addOperator(LuaTokenTypes::andKeyword, 1, Associativity::left);
	addOperator(LuaTokenTypes::doubleAmpersand, 1, Associativity::left);

	addOperator(LuaTokenTypes::less, 2, Associativity::left);
	addOperator(LuaTokenTypes::greater, 2, Associativity::left);
	addOperator(LuaTokenTypes::lessEquals, 2, Associativity::left);
	addOperator(LuaTokenTypes::greaterEquals, 2, Associativity::left);
	addOperator(LuaTokenTypes::tildaEquals, 2, Associativity::left);
	addOperator(LuaTokenTypes::exclamationMarkEquals, 2, Associativity::left);
	addOperator(LuaTokenTypes::doubleEquals, 2, Associativity::left);

	addOperator(LuaTokenTypes::verticalLine, 3, Associativity::left);

	addOperator(LuaTokenTypes::tilda, 4, Associativity::left);

	addOperator(LuaTokenTypes::ampersand, 5, Associativity::left);

	addOperator(LuaTokenTypes::doubleLess, 6, Associativity::left);
	addOperator(LuaTokenTypes::doubleGreater, 6, Associativity::left);

	addOperator(LuaTokenTypes::doubleDot, 7, Associativity::right);

	addOperator(LuaTokenTypes::plus, 8, Associativity::left);
	addOperator(LuaTokenTypes::minus, 8, Associativity::left);

	addOperator(LuaTokenTypes::asterick, 9, Associativity::left);
	addOperator(LuaTokenTypes::slash, 9, Associativity::left);
	addOperator(LuaTokenTypes::doubleSlash, 9, Associativity::left);
	addOperator(LuaTokenTypes::percent, 9, Associativity::left);

	addOperator(LuaTokenTypes::notKeyword, 10);
	addOperator(LuaTokenTypes::sharp, 10);
	addOperator(LuaTokenTypes::minus, 10);
	addOperator(LuaTokenTypes::tilda, 10);

	addOperator(LuaTokenTypes::hat, 11, Associativity::right);
}
