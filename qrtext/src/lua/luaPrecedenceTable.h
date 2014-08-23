#pragma once

#include <QtCore/QScopedPointer>

#include "qrtext/core/parser/precedenceTable.h"

#include "qrtext/src/lua/luaTokenTypes.h"

namespace qrtext {
namespace lua {
namespace details {

class LuaPrecedenceTable : public core::PrecedenceTable<LuaTokenTypes>
{
public:
	LuaPrecedenceTable();
};

}
}
}
