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
	} else if (type1->is<Boolean>() && type2->is<Boolean>()) {
		return true;
	} else if (type1->is<Float>() && type2->is<Float>()) {
		return true;
	} else if (type1->is<Integer>() && type2->is<Integer>()) {
		return true;
	} else if (type1->is<Nil>() && type2->is<Nil>()) {
		return true;
	} else if (type1->is<String>() && type2->is<String>()) {
		return true;
	} else if (type1->is<Table>() && type2->is<Table>()) {
		return isStructurallyEquivalent(type1.dynamicCast<Table>()->elementType()
				, type2.dynamicCast<Table>()->elementType());
	} else if (type1->is<Function>() && type2->is<Function>()) {
		/// @todo: Actually compare function types.
		return true;
	}

	return false;
}
