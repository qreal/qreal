#include "qrtext/src/lua/luaGeneralizationsTable.h"

#include "qrtext/core/types/any.h"
#include "qrtext/lua/types/boolean.h"
#include "qrtext/lua/types/float.h"
#include "qrtext/lua/types/function.h"
#include "qrtext/lua/types/integer.h"
#include "qrtext/lua/types/nil.h"
#include "qrtext/lua/types/number.h"
#include "qrtext/lua/types/string.h"
#include "qrtext/lua/types/table.h"

using namespace qrtext::lua::details;
using namespace qrtext::core::types;
using namespace qrtext::lua::types;

bool LuaGeneralizationsTable::isGeneralization(const QSharedPointer<TypeExpression> &specific
		, const QSharedPointer<TypeExpression> &general) const
{
	return (specific->is<Integer>() && general->is<Float>()) || isStructurallyEquivalent(specific, general);
}

bool LuaGeneralizationsTable::isStructurallyEquivalent(const QSharedPointer<TypeExpression> &type1
		, const QSharedPointer<TypeExpression> &type2) const
{
	if (type1->is<Any>() && type2->is<Any>()) {
		return true;
	} if (type1->is<Boolean>() && type2->is<Boolean>()) {
		return true;
	} if (type1->is<Float>() && type2->is<Float>()) {
		return true;
	} if (type1->is<Integer>() && type2->is<Integer>()) {
		return true;
	} if (type1->is<Nil>() && type2->is<Nil>()) {
		return true;
	} if (type1->is<String>() && type2->is<String>()) {
		return true;
	}

	return false;
}
