#pragma once

#include "../../../models/models.h"
#include "../../../../qrrepo/commonRepoApi.h"
#include "diffState.h"
#include "modification.h"

namespace versioning
{
namespace details
{

class CommonDifference
{
public:
	CommonDifference(qReal::models::Models *oldModel
			, qReal::models::Models *newModel, qReal::Id const &id);
	virtual ~CommonDifference() {}

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
	qReal::models::Models *mOldModel;
	qReal::models::Models *mNewModel;

	void findCommonDifference();
	virtual qrRepo::CommonRepoApi const &repoApi(bool old) const = 0;

private:
	void findChildrenDifference();
	void findPropertyDifference();
	void findPropertyDifference(bool old);

	static bool compareProperties(QVariant const &property1, QVariant const &property2);

	qReal::Id mId;
	DiffState mState;

	DiffState mParentState;
	qReal::Id mOldParent;
	qReal::Id mNewParent;

	DiffState mChildrenState;
	QHash<qReal::Id, DiffState> mChildren;
	DiffState mPropertiesState;
	QHash<QString, Modification> mProperties;

};

}
}
