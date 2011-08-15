#include "diffprovider.h"
#include "difference.h"

using namespace qReal;
using namespace qReal::diffManager::details;

DiffProvider::DiffProvider(models::Models *oldModel, models::Models *newModel)
	: mOldModel(oldModel), mNewModel(newModel),
	mOldGraphicalElements(graphicalElements(mOldModel)),
	mNewGraphicalElements(graphicalElements(mNewModel)),
	mAllGraphicalElements(uniteIdLists(mOldGraphicalElements, mNewGraphicalElements)),
	mDifferences()
{
	findDifferences();
}

Difference *DiffProvider::difference(const Id &id) const
{
	qReal::Id logicalId = this->logicalId(id);
	if (mDifferences.keys().contains(logicalId))
		return mDifferences[logicalId];
	else
		return NULL;
}

models::Models *DiffProvider::oldModel() const
{
	return mOldModel;
}

models::Models *DiffProvider::newModel() const
{
	return mNewModel;
}

qReal::IdList DiffProvider::oldGraphicalElements() const
{
	return mOldGraphicalElements;
}

qReal::IdList DiffProvider::newGraphicalElements() const
{
	return mNewGraphicalElements;
}

qReal::Id DiffProvider::logicalId(const Id &id) const
{
	if ((mOldModel->logicalRepoApi().exist(id) && mOldModel->logicalRepoApi().isLogicalElement(id))
		|| (mNewModel->logicalRepoApi().exist(id) && mNewModel->logicalRepoApi().isLogicalElement(id)))
	{
		return id;
	}
	if (mOldGraphicalElements.contains(id))
	{
		return mOldModel->graphicalRepoApi().logicalId(id);
	}
	if (mNewGraphicalElements.contains(id))
	{
		return mNewModel->graphicalRepoApi().logicalId(id);
	}
	return Id();
}

ElementType DiffProvider::elementType(const Id &id) const
{
	if (mAllGraphicalElements.contains(id))
	{
		return (logicalId(id) != qReal::Id()) ? GraphicalAndLogical : PurelyGraphical;
	}
	return (logicalId(id) != qReal::Id()) ? PurelyLogical : Nonexistent;
}

qReal::IdList DiffProvider::graphicalElements(models::Models *model)
{
	qReal::IdList result;
	graphicalElements(model, result, model->graphicalModelAssistApi().rootId());
	return result;
}

void DiffProvider::graphicalElements(models::Models *model, qReal::IdList &result, const Id &parent)
{
	if (model->graphicalModelAssistApi().isGraphicalId(parent))
		result.append(parent);
	foreach (qReal::Id child, model->graphicalRepoApi().children(parent))
	{
		graphicalElements(model, result, child);
	}
}

qReal::IdList DiffProvider::uniteIdLists(const IdList &list1, const IdList &list2)
{
	qReal::IdList result(list1);
	foreach (qReal::Id id, list2)
	{
		if (!list1.contains(id))
			result.append(id);
	}
	return result;
}

void DiffProvider::findDifferences()
{
	foreach(Id id, mAllGraphicalElements)
	{
		Id logicalId = this->logicalId(id);
		mDifferences.insert(logicalId, new Difference(mOldModel, mNewModel, id, logicalId, elementType(id)));
	}
}
