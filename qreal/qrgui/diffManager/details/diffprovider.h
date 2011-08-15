#pragma once

#include "difference.h"
#include "elementtype.h"
#include "../models/models.h"

namespace qReal
{
namespace diffManager
{
namespace details
{

class DiffProvider
{
public:
	DiffProvider(models::Models *oldModel, models::Models *newModel);
	Difference *difference(qReal::Id const &id) const;
	models::Models *oldModel() const;
	models::Models *newModel() const;
	qReal::IdList oldGraphicalElements() const;
	qReal::IdList newGraphicalElements() const;
	qReal::Id logicalId(qReal::Id const &id) const;
	ElementType elementType(qReal::Id const &id) const;

private:
	models::Models *mOldModel;
	models::Models *mNewModel;
	qReal::IdList mOldGraphicalElements;
	qReal::IdList mNewGraphicalElements;
	qReal::IdList mAllGraphicalElements;
	QHash<qReal::Id, Difference*> mDifferences;

	qReal::IdList graphicalElements(models::Models *model);
	void graphicalElements(models::Models *model, qReal::IdList &result, qReal::Id const &parent);
	qReal::IdList uniteIdLists(qReal::IdList const &list1, qReal::IdList const &list2);
	void findDifferences();
};

}
}
}
