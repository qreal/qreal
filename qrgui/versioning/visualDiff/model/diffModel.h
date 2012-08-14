#pragma once

#include "difference.h"
#include "elementType.h"

namespace versioning
{

class DiffModel
{
public:
	DiffModel(qReal::models::Models *oldModel, qReal::models::Models *newModel);

	details::Difference *difference(qReal::Id const &id) const;
	qReal::models::Models *oldModel() const;
	qReal::models::Models *newModel() const;
	qReal::IdList oldGraphicalElements() const;
	qReal::IdList newGraphicalElements() const;
	qReal::Id logicalId(qReal::Id const &id) const;
	details::ElementType elementType(qReal::Id const &id) const;

private:
	qReal::IdList graphicalElements(qReal::models::Models *model);
	void graphicalElements(qReal::models::Models *model, qReal::IdList &result, qReal::Id const &parent);
	qReal::IdList uniteIdLists(qReal::IdList const &list1, qReal::IdList const &list2);
	void findDifferences();

	qReal::models::Models *mOldModel;
	qReal::models::Models *mNewModel;
	qReal::IdList mOldGraphicalElements;
	qReal::IdList mNewGraphicalElements;
	qReal::IdList mAllGraphicalElements;
	QHash<qReal::Id, details::Difference *> mDifferences;
};

}
