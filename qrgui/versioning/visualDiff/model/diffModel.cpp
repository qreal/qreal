#include "diffModel.h"
#include "difference.h"

using namespace versioning;
using namespace versioning::details;

DiffModel::DiffModel(qReal::models::Models *oldModel
		, qReal::models::Models *newModel)
	: mOldModel(oldModel), mNewModel(newModel)
	, mOldGraphicalElements(graphicalElements(mOldModel))
	, mNewGraphicalElements(graphicalElements(mNewModel))
	, mAllGraphicalElements(uniteIdLists(mOldGraphicalElements, mNewGraphicalElements))
	, mDifferences()
{
	findDifferences();
}

Difference *DiffModel::difference(const qReal::Id &id) const
{
	qReal::Id logicalId = this->logicalId(id);
	if (mDifferences.keys().contains(logicalId)) {
		return mDifferences[logicalId];
	} else {
		return NULL;
	}
}

qReal::models::Models *DiffModel::oldModel() const
{
	return mOldModel;
}

qReal::models::Models *DiffModel::newModel() const
{
	return mNewModel;
}

qReal::IdList DiffModel::oldGraphicalElements() const
{
	return mOldGraphicalElements;
}

qReal::IdList DiffModel::newGraphicalElements() const
{
	return mNewGraphicalElements;
}

qReal::Id DiffModel::logicalId(const qReal::Id &id) const
{
	if ((mOldModel->logicalRepoApi().exist(id) && mOldModel->logicalRepoApi().isLogicalElement(id))
		|| (mNewModel->logicalRepoApi().exist(id) && mNewModel->logicalRepoApi().isLogicalElement(id))) {
		return id;
	}
	if (mOldGraphicalElements.contains(id)) {
		return mOldModel->graphicalRepoApi().logicalId(id);
	}
	if (mNewGraphicalElements.contains(id)) {
		return mNewModel->graphicalRepoApi().logicalId(id);
	}
	return qReal::Id();
}

ElementType DiffModel::elementType(const qReal::Id &id) const
{
	if (mAllGraphicalElements.contains(id)) {
		return (logicalId(id) != qReal::Id()) ? GraphicalAndLogical : PurelyGraphical;
	}
	return (logicalId(id) != qReal::Id()) ? PurelyLogical : Nonexistent;
}

qReal::IdList DiffModel::graphicalElements(qReal::models::Models *model)
{
	qReal::IdList result;
	graphicalElements(model, result, model->graphicalModelAssistApi().rootId());
	return result;
}

void DiffModel::graphicalElements(qReal::models::Models *model, qReal::IdList &result, const qReal::Id &parent)
{
	if (model->graphicalModelAssistApi().isGraphicalId(parent)) {
		result.append(parent);
	}
	foreach (qReal::Id const &child, model->graphicalRepoApi().children(parent)) {
		graphicalElements(model, result, child);
	}
}

qReal::IdList DiffModel::uniteIdLists(const qReal::IdList &list1, const qReal::IdList &list2)
{
	qReal::IdList result(list1);
	foreach (qReal::Id const &id, list2) {
		if (!list1.contains(id)) {
			result.append(id);
		}
	}
	return result;
}

void DiffModel::findDifferences()
{
	foreach(qReal::Id const &id, mAllGraphicalElements) {
		qReal::Id const logicalId = this->logicalId(id);
		mDifferences.insert(logicalId, new Difference(mOldModel, mNewModel, id, logicalId, elementType(id)));
	}
}
