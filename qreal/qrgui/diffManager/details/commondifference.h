#pragma once

#include "../../qrrepo/commonrepoapi.h"
#include "../../qrgui/kernel/ids.h"
#include "../models/models.h"
#include "diffState.h"
#include "modification.h"

namespace qReal
{
namespace diffManager
{
namespace details
{
class CommonDifference
{
public:
	CommonDifference(models::Models *oldModel, models::Models *newModel, qReal::Id const &id);

	qReal::Id id() const;
	DiffState state() const;
	DiffState parentState() const;
	qReal::Id oldParent() const;
	qReal::Id newParent() const;

	DiffState childrenState() const;
	DiffState propertiesState() const;
	qReal::IdList allChidren() const;
	DiffState childState(qReal::Id const &id) const;
	QListIterator<QString> propertyNamesIterator() const;
	DiffState propertyState(QString const &name) const;
	QVariant oldProperty(QString const &name) const;
	QVariant newProperty(QString const &name) const;

protected:
	models::Models *mOldModel;
	models::Models *mNewModel;

	void findCommonDifference();
	virtual qrRepo::CommonRepoApi const &repoApi(bool old) const = 0;

private:
	qReal::Id mId;
	DiffState mState;

	DiffState mParentState;
	qReal::Id mOldParent;
	qReal::Id mNewParent;

	DiffState mChildrenState;
	QHash<qReal::Id, DiffState> mChildren;
	DiffState mPropertiesState;
	QHash<QString, Modification> mProperties;

	void findChildrenDifference();
	void findPropertyDifference();
	void findPropertyDifference(bool old);
	bool compareProperties(QVariant const &property1, QVariant const &property2) const;

};

}
}
}
