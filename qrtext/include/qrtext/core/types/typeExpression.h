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

#include <QtCore/QList>
#include <QtCore/QSharedPointer>

#include "qrtext/declSpec.h"

namespace qrtext {
namespace core {
namespace types {

/// Base class for all type expressions.
class QRTEXT_EXPORT TypeExpression
{
public:
	virtual ~TypeExpression() {}

	/// Returns true, if this type expression is a subclass of given, in C++ terms, not in target language terms. One
	/// can use C++ inheritance to express generalization relation of a target language, but since C++ does not support
	/// variance it is not possible to do so for complex types. Then GeneralizationsTableInterface shall be used.
	template<typename Type>
	bool is() const
	{
		return dynamic_cast<const Type * const>(this) != nullptr;
	}

	/// Returns string representation of type expression, supposed to be used for error reporting.
	virtual QString toString() const = 0;
};

}

/// Helper function that wraps given type expression into smart pointer.
inline QSharedPointer<types::TypeExpression> wrap(types::TypeExpression *type)
{
	return QSharedPointer<types::TypeExpression>(type);
}

}
}
