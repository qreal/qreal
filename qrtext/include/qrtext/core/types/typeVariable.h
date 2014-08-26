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
class QRTEXT_EXPORT TypeVariable : public TypeExpression {
public:
	/// Creates a variable that can contain any type.
	TypeVariable();

	/// Creates already resolved variable that can contain only one type, but it can become empty later.
	TypeVariable(QSharedPointer<types::TypeExpression> const &singleType);

	/// Returns true if a variable can be of only one type (so its type is known).
	bool isResolved() const;

	/// Returns true if a variable can not be of any type (so it is most likely a type error).
	bool isEmpty() const;

	/// Returns a type of a resolved variable.
	QSharedPointer<types::TypeExpression> finalType() const;

	/// Constrains a variable with possible types of other variable, with respect of given generalizations table.
	/// If a variable can not contain any of other variable's types, then it will be generalized to less general type
	/// that can. For example, in "a<int> = 0.5<float>;" identifier "a" will be generalized to type "float".
	/// Note that existing type constraints, like usage of "a" in modulo operation, will be broken by generalization,
	/// this is a limitation of a current algorithm.
	/// @todo Fix generalization issue, maybe by assigning <int, float> to integer constants and using less general type
	///       as final.
	void constrain(QSharedPointer<TypeVariable> const &other, GeneralizationsTableInterface const &generalizationsTable);

	/// Constrains a variable with a list of possible types, with respect of given generalizations table. Follows the
	/// same rules as other overload of constrain().
	void constrain(QList<QSharedPointer<TypeExpression>> const &types
			, GeneralizationsTableInterface const &generalizationsTable);

	void constrainAssignment(QSharedPointer<TypeVariable> const &other
			, GeneralizationsTableInterface const &generalizationsTable
			, bool *wasCoercion);

private:
	QSet<QSharedPointer<TypeExpression>> mAllowedTypes;
};

}
}
}
