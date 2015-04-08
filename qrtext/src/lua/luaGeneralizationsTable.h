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

#include "qrtext/core/semantics/generalizationsTableInterface.h"

namespace qrtext {
namespace lua {
namespace details {

/// Table of type-subtype relations for Lua language.
class LuaGeneralizationsTable : public core::GeneralizationsTableInterface
{
public:
	bool isGeneralization(QSharedPointer<core::types::TypeExpression> const &specific
			, QSharedPointer<core::types::TypeExpression> const &general) const override;

private:
	bool isStructurallyEquivalent(QSharedPointer<core::types::TypeExpression> const &type1
			, QSharedPointer<core::types::TypeExpression> const &type2) const;
};

}
}
}
