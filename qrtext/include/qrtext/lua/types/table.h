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

#include "qrtext/core/types/typeExpression.h"

namespace qrtext {
namespace lua {
namespace types {

/// Represents table type.
class Table : public core::types::TypeExpression
{
public:
	/// Constructor.
	/// @param elementType - type of table element.
	/// @param size - size of a table.
	Table(QSharedPointer<core::types::TypeExpression> const &elementType
			, int size)
		: mElementType(elementType), mSize(size)
	{
	}

	/// Returns type of table element.
	QSharedPointer<core::types::TypeExpression> const &elementType() const
	{
		return mElementType;
	}

	QString toString() const override
	{
		return QObject::tr("table[%1]").arg(mElementType->toString());
	}

private:
	QSharedPointer<core::types::TypeExpression> mElementType;
	int mSize;
};

}
}
}
