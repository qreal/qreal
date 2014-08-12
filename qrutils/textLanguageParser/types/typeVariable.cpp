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

QSharedPointer<TypeExpression> TypeVariable::finalType() const
{
	return mAllowedTypes.toList()[0].dynamicCast<TypeExpression>();
}

