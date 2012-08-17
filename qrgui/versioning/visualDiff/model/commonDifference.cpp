#include "commonDifference.h"

using namespace versioning::details;

//repo <-> old object
//working copy <-> new object

CommonDifference::CommonDifference(qReal::models::Models *oldModel
		, qReal::models::Models *newModel, qReal::Id const &id)
	: mOldModel(oldModel), mNewModel(newModel), mId(id)
{
}

qReal::Id CommonDifference::id() const
{
	return mId;
}

DiffState CommonDifference::state() const
{
	return mState;
}

DiffState CommonDifference::parentState() const
{
	return mParentState;
}

qReal::Id CommonDifference::oldParent() const
{
	return mOldParent;
}

qReal::Id CommonDifference::newParent() const
{
	return mNewParent;
}

DiffState CommonDifference::childrenState() const
{
	return mChildrenState;
}

DiffState CommonDifference::propertiesState() const
{
	return mPropertiesState;
}

qReal::IdList CommonDifference::allChidren() const
{
	switch(mChildrenState) {
	case Same:
	case Modified:
		return mChildren.keys();
	case Added:
		return repoApi(false).children(mId);
	case Removed:
		return repoApi(true).children(mId);
	default:
		qDebug() << "Unknown Diff State";
		qDebug() << mChildrenState;
		return qReal::IdList();
	}

}

DiffState CommonDifference::childState(const qReal::Id &id) const
{
	if (Added == mChildrenState || Removed == mChildrenState) {
		return mChildrenState;
	}
	return mChildren[id];
}

QListIterator<QString> CommonDifference::propertyNamesIterator() const
{
	return QListIterator<QString>(mProperties.keys());
}

DiffState CommonDifference::propertyState(const QString &name) const
{
	DiffState const generalState = propertiesState();
	if (Added == generalState || Removed == generalState) {
		return generalState;
	}
	return mProperties[name].state();
}

QVariant CommonDifference::oldProperty(const QString &name) const
{
	return mProperties[name].oldValue();
}

QVariant CommonDifference::newProperty(const QString &name) const
{
	return mProperties[name].newValue();
}

void CommonDifference::findCommonDifference()
{
	if (repoApi(true).exist(mId)) {
		if (repoApi(false).exist(mId)) {
			mOldParent = repoApi(true).parent(mId);
			mNewParent = repoApi(false).parent(mId);
			mParentState = (mOldParent == mNewParent) ? Same : Modified;
			findChildrenDifference();
			findPropertyDifference();
			mState = (mParentState == Same && mChildrenState == Same && mPropertiesState == Same)
					? Same : Modified;
		} else {
			mState = mParentState = mChildrenState = mPropertiesState = Removed;
			mOldParent = repoApi(true).parent(mId);
			findPropertyDifference(true);
		}
	} else {
		if (repoApi(false).exist(mId)) {
			mState = mParentState = mChildrenState = mPropertiesState = Added;
			mNewParent = repoApi(false).parent(mId);
			findPropertyDifference(false);
		} else {
			qDebug() << "Trying to find difference bewtween two nonexisting objects (id: " + mId.toString() + ")";
		}
	}
}

void CommonDifference::findChildrenDifference()
{
	bool hasModifications = false;
	qReal::IdList oldChildren = repoApi(true).children(mId);
	qReal::IdList newChildren = repoApi(false).children(mId);
	foreach (qReal::Id const &child, oldChildren) {
		DiffState const state = (newChildren.contains(child)) ? Same : Removed;
		if (state == Removed) {
			hasModifications = true;
		}
		mChildren.insert(child, state);
	}
	foreach (qReal::Id const &child, newChildren) {
		if (!mChildren.keys().contains(child)) {
			mChildren.insert(child, Added);
			hasModifications = true;
		}
	}
	mChildrenState = hasModifications ? Modified : Same;
}

void CommonDifference::findPropertyDifference()
{
	bool hasModifications = false;
	QMapIterator<QString, QVariant> i = repoApi(true).propertiesIterator(mId);
	while (i.hasNext()) {
		i.next();
		QString key = i.key();
		QVariant const oldValue = i.value();
		QVariant const newValue = repoApi(false).property(mId, key);
		Modification modification;
		modification.setOldValue(oldValue);
		modification.setNewValue(newValue);
		DiffState const state = (compareProperties(oldValue, newValue))
				? Same : Modified;
		if (state == Modified) {
			hasModifications = true;
		}
		modification.setState(state);
		mProperties.insert(key, modification);
	}
	mPropertiesState = hasModifications ? Modified : Same;
}

void CommonDifference::findPropertyDifference(bool old)
{
	QMapIterator<QString, QVariant> i = repoApi(old).propertiesIterator(mId);
	while (i.hasNext()) {
		i.next();
		QString const key = i.key();
		QVariant const value = i.value();
		Modification modification;
		modification.setOldValue(value);
		modification.setNewValue(value);
		DiffState const state = (old) ? Removed : Added;
		modification.setState(state);
		mProperties.insert(key, modification);
	}
}

bool CommonDifference::compareProperties(QVariant const &property1, QVariant const &property2)
{
	if (property1.type() != property2.type()) {
		return false;
	}
	if (property1.type() == QVariant::UserType) {
		if (property1.userType() == QMetaType::type("qReal::Id")) {
			return property1.value<qReal::Id>() == property2.value<qReal::Id>();
		}
		if (property1.userType() == QMetaType::type("qReal::IdList")) {
			return property1.value<qReal::IdList>() == property2.value<qReal::IdList>();
		}
		qDebug() << "Unsupported QVariant type while comparing properties.";
		qDebug() << property1;
	}
	return property1 == property2;
}
