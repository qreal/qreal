#include "luaPrecedenceTable.h"

using namespace qrtext::lua::details;
using namespace qrtext::core::parser;

LuaPrecedenceTable::LuaPrecedenceTable()
{
	addOperator(LuaTokenTypes::orKeyword, 0, Associativity::left);
	addOperator(LuaTokenTypes::andKeyword, 1, Associativity::left);

	addOperator(LuaTokenTypes::less, 2, Associativity::left);
	addOperator(LuaTokenTypes::greater, 2, Associativity::left);
	addOperator(LuaTokenTypes::lessEquals, 2, Associativity::left);
	addOperator(LuaTokenTypes::greaterEquals, 2, Associativity::left);
	addOperator(LuaTokenTypes::tildaEquals, 2, Associativity::left);
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
