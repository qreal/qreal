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

#include <QtCore/QSet>
#include <QtCore/QSharedPointer>

#include "qrtext/core/types/typeExpression.h"
#include "qrtext/core/semantics/generalizationsTableInterface.h"

namespace qrtext {
namespace core {
namespace types {

/// A type that can be one of a set of types. Type variable can be constrained by a given set of types, reducing its
/// possiblities. It can be constrained "too much", so there are no valid types for this variable, then it is called
/// "empty". When variable can be of only one type, it is called "resolved".
class QRTEXT_EXPORT TypeVariable : public TypeExpression
{
public:
	/// Creates a variable that can contain any type.
	TypeVariable();

	/// Creates already resolved variable that can contain only one type, but it can become empty later.
	TypeVariable(const QSharedPointer<types::TypeExpression> &singleType);

	/// Returns true if a variable can be of only one type (so its type is known).
	bool isResolved() const;

	/// Returns true if a variable can not be of any type (so it is most likely a type error).
	bool isEmpty() const;

	/// Returns a type of a resolved variable.
	QSharedPointer<types::TypeExpression> finalType() const;

	/// Constrains a variable with possible types of other variable, with respect of given generalizations table.
	void constrain(const QSharedPointer<TypeVariable> &other
			, const GeneralizationsTableInterface &generalizationsTable);

	/// Constrains a variable with a list of possible types, with respect of given generalizations table. Follows the
	/// same rules as other overload of constrain().
	void constrain(const QList<QSharedPointer<TypeExpression>> &types
			, const GeneralizationsTableInterface &generalizationsTable);

	/// Constrains an assignment with respect of given generalizations table.
	/// If a variable can not contain any of other variable's types, then it will be generalized to closest more general
	/// type that can. For example, in "a<int> = 0.5<float>;" identifier "a" will be generalized to type "float".
	void constrainAssignment(const QSharedPointer<TypeVariable> &other
			, const GeneralizationsTableInterface &generalizationsTable
			, bool *wasCoercion);

	QString toString() const override;

private:
	QSet<QSharedPointer<TypeExpression>> mAllowedTypes;
};

}
}
}
