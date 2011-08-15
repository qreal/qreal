#include "modification.h"

using namespace qReal::diffManager::details;

Modification::Modification()
{
}

DiffState Modification::state() const
{
	return mState;
}

QVariant Modification::oldValue() const
{
	return mOldValue;
}

QVariant Modification::newValue() const
{
	return mNewValue;
}

void Modification::setState(DiffState const state)
{
	mState = state;
}

void Modification::setOldValue(QVariant const &value)
{
	mOldValue = value;
}

void Modification::setNewValue(QVariant const &value)
{
	mNewValue = value;
}
