#include "currentScope.h"

using namespace generationRules::generator;

CurrentScope::CurrentScope()
	: mStackOfThis()
	, mCurrentGeneratorName()
{
}

void CurrentScope::changeCurrentId(const qReal::Id &newCurrentVariableId)
{
	mStackOfThis.push(newCurrentVariableId);
}

qReal::Id CurrentScope::currentId() const
{
	return mStackOfThis.top();
}

void CurrentScope::removeLastCurrentId()
{
	mStackOfThis.pop();
}

void CurrentScope::changeCurrentGeneratorName(const QString &newGeneratorName)
{
	mCurrentGeneratorName = newGeneratorName;
}

QString CurrentScope::currentGeneratorName() const
{
	return mCurrentGeneratorName;
}
