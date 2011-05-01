#include "modification.h"

using namespace qrRepo::details;

Modification::Modification()
{
}

DiffState Modification::state()
{
	return mState;
}

QVariant Modification::oldValue()
{
	return mOldValue;
}

QVariant Modification::newValue()
{
	return mNewValue;
}

void Modification::setState(DiffState state)
{
	mState = state;
}

void Modification::setOldValue(QVariant value)
{
	mOldValue = value;
}

void Modification::setNewValue(QVariant value)
{
	mNewValue = value;
}
