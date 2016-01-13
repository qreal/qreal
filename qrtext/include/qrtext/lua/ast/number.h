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

#include "qrtext/lua/ast/expression.h"

namespace qrtext {
namespace lua {
namespace ast {

/// Base class for integer and float numbers.
class QRTEXT_EXPORT Number : public Expression
{
public:
	/// Constructor.
	/// @param stringRepresentation - string representation of a number. Actual value shall be calculated by
	/// interpreter.
	Number(const QString &stringRepresentation)
		: mStringRepresentation(stringRepresentation)
	{
	}

	/// Returns string representation of a number.
	const QString &stringRepresentation() const {
		return mStringRepresentation;
	}

private:
	void accept(core::AstVisitorInterface &visitor, const QSharedPointer<Node> &pointer
			, const QSharedPointer<Node> &parent) override
	{
		static_cast<LuaAstVisitorInterface *>(&visitor)->visit(qSharedPointerCast<Number>(pointer), parent);
	}

	QString mStringRepresentation;
};

}
}
}
