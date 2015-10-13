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

#include "qrtext/core/types/typeVariable.h"

#include <QtCore/QStringList>

#include "qrtext/core/types/any.h"

using namespace qrtext::core::types;

TypeVariable::TypeVariable()
{
	mAllowedTypes << QSharedPointer<TypeExpression>(new Any());
}

TypeVariable::TypeVariable(const QSharedPointer<TypeExpression> &singleType)
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
	if (mAllowedTypes.size() == 0) {
		return QSharedPointer<TypeExpression>(new Any());
	} else {
		return mAllowedTypes.toList()[0].dynamicCast<TypeExpression>();
	}
}

void TypeVariable::constrain(const QSharedPointer<TypeVariable> &other
		, const GeneralizationsTableInterface &generalizationsTable)
{
	constrain(other->mAllowedTypes.toList(), generalizationsTable);
}

void TypeVariable::constrain(const QList<QSharedPointer<TypeExpression>> &types
		, const GeneralizationsTableInterface &generalizationsTable)
{
	QSet<QSharedPointer<TypeExpression>> result;

	for (auto type : mAllowedTypes) {
		for (auto otherType : types) {
			if (type->is<Any>()) {
				result << otherType;
			} else if (otherType->is<Any>()) {
				result << type;
			} else if (generalizationsTable.isGeneralization(type, otherType)) {
				result << type;
			}
		}
	}

	mAllowedTypes = result;
}

void TypeVariable::constrainAssignment(const QSharedPointer<TypeVariable> &other
		, const GeneralizationsTableInterface &generalizationsTable
		, bool *wasCoercion)
{
	*wasCoercion = false;
	QSet<QSharedPointer<TypeExpression>> result;

	for (auto type : mAllowedTypes) {
		QSet<QSharedPointer<TypeExpression>> allowedForType;

		for (auto otherType : other->mAllowedTypes) {
			if (type->is<Any>()) {
				allowedForType << otherType;
			} else if (otherType->is<Any>()) {
				result << type;
			} else if (generalizationsTable.isGeneralization(otherType, type)) {
				allowedForType << type;
			}
		}

		// If there is no valid subtype for our type between types, maybe it is possible
		// to promote our type using language coercion rules.
		// For example, if we assign float value to integer variable and a language
		// allows implicit casts from integer to float, we'll make our variable float.
		if (allowedForType.isEmpty()) {
			for (auto otherType : other->mAllowedTypes) {
				if (generalizationsTable.isGeneralization(type, otherType)) {
					result << otherType;
					*wasCoercion = true;
				}
			}
		}

		result.unite(allowedForType);
	}

	mAllowedTypes = result;
}

QString TypeVariable::toString() const
{
	QStringList result;
	for (const QSharedPointer<types::TypeExpression> &type : mAllowedTypes) {
		result.append(type->toString());
	}

	return result.join(", ");
}
