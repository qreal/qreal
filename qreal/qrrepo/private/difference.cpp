#include "difference.h"

using namespace qrRepo::details;

Difference::Difference(Object *objectInRepo, Object *objectInWorkingCopy)
{
	mOldObject = objectInRepo;
	mNewObject = objectInWorkingCopy;
	mId = mOldObject->id();
	mLogicId = mOldObject->logicalId();
	mOldParent = mOldObject->parent();
	mNewParent = mNewObject->parent();
	mParentState = (mOldParent == mNewParent) ? Same : Modified;
	findChildrenDifference();
	findPropertyDifference();
	mState = (mParentState == Same && mChildrenState == Same && mPropertyState == Same) ? Same : Modified;
}

Difference::Difference(Object *object, DiffState state)
{
	switch (state)
	{
	case Added:
		mOldObject = NULL;
		mNewObject = object;
		break;
	case Removed:
		mOldObject = object;
		mNewObject = NULL;
		break;
	default: //Invalid constructor usage.
		mOldObject = NULL;
		mNewObject = NULL;
		break;
	}
	mState = state;
}

void Difference::findChildrenDifference()
{
	bool hasModifications = false;
	qReal::IdList oldChildren = mOldObject->children();
	qReal::IdList newChildren = mNewObject->children();
	foreach (qReal::Id child, oldChildren)
	{
		DiffState state = (newChildren.contains(child)) ? Same : Removed;
		if (state == Removed)
			hasModifications = true;
		mChildren.insert(child, state);
	}
	foreach (qReal::Id child, newChildren)
	{
		if (!mChildren.keys().contains(child))
		{
			mChildren.insert(child, Added);
			hasModifications = true;
		}
	}
	mChildrenState = hasModifications ? Modified : Same;
}

void Difference::findPropertyDifference()
{
	bool hasModifications = false;
	QMapIterator<QString, QVariant> i = mOldObject->propertiesIterator();
	while (i.hasNext())
	{
		i.next();
		QString key = i.key();
		QVariant oldValue = i.value();
		QVariant newValue = mNewObject->property(key);
		Modification modification;
		modification.setOldValue(oldValue);
		modification.setNewValue(newValue);
		DiffState state = (oldValue == newValue) ? Same : Modified;
		if (state == Modified)
			hasModifications = true;
		modification.setState(state);
		mProperties.insert(key, modification);
	}
	mPropertyState = hasModifications ? Modified : Same;
}
