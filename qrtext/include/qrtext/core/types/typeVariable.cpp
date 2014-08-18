#include "textLanguageParser/types/typeVariable.h"

#include "textLanguageParser/types/any.h"

using namespace textLanguageParser::types;

TypeVariable::TypeVariable()
{
	mAllowedTypes << QSharedPointer<TypeExpression>(new Any());
}

TypeVariable::TypeVariable(QSharedPointer<TypeExpression> const &singleType)
{
	mAllowedTypes << singleType;
}

bool TypeVariable::isResolved() const
{
	return mAllowedTypes.size() == 1 && !mAllowedTypes.toList().first()->is<Any>();
}

bool TypeVariable::isEmpty() const
{
	return mAllowedTypes.isEmpty();
}

QSharedPointer<TypeExpression> TypeVariable::finalType() const
{
	return mAllowedTypes.toList()[0].dynamicCast<TypeExpression>();
}

void TypeVariable::constrain(QSharedPointer<TypeVariable> const &other
		, GeneralizationsTable const &generalizationsTable)
{
	constrain(other->mAllowedTypes.toList(), generalizationsTable);
}

void TypeVariable::constrain(QList<QSharedPointer<TypeExpression>> const &types
		, GeneralizationsTable const &generalizationsTable)
{
	QSet<QSharedPointer<TypeExpression>> result;

	for (auto type : mAllowedTypes) {
		QSet<QSharedPointer<TypeExpression>> allowedForType;

		for (auto otherType : types) {
			if (type->is<Any>()) {
				allowedForType << otherType;
			} else if (generalizationsTable.isGeneralization(otherType, type)) {
				allowedForType << type;
			}
		}

		// If there is no valid subtype for our type between types, maybe it is possible
		// to promote our type using language coercion rules.
		// For example, if we assign float value to integer variable and a language
		// allows implicit casts from integer to float, we'll make our variable float.
		if (allowedForType.isEmpty()) {
			for (auto otherType : types) {
				if (generalizationsTable.isGeneralization(type, otherType)) {
					result << otherType;
				}
			}
		}

		result.unite(allowedForType);
	}

	mAllowedTypes = result;
}
