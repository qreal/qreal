#include "graphicalPartModel.h"

using namespace qReal;
using namespace qReal::models::details;
using namespace qReal::models::details::modelsImplementation;

GraphicalPartModel::GraphicalPartModel(qrRepo::GraphicalRepoApi &repoApi)
	: mRepoApi(repoApi)
{
}

GraphicalPartModel::~GraphicalPartModel()
{
	typedef QList<modelsImplementation::GraphicalPartModelItem *> GraphicalPartModelItemList;
	foreach (GraphicalPartModelItemList const &list, mItems) {
		qDeleteAll(list);
	}
}

QVariant GraphicalPartModel::data(QModelIndex const &index, int role) const
{
	GraphicalPartModelItem const * const item = static_cast<GraphicalPartModelItem const *>(index.internalPointer());
	switch (role) {
	case positionRole:
		return mRepoApi.graphicalPartProperty(item->id(), item->index(), "position");
	case configurationRole:
		return mRepoApi.graphicalPartProperty(item->id(), item->index(), "configuration");
	default:
		return QVariant();
	}
}

bool GraphicalPartModel::setData(QModelIndex const &index, QVariant const &value, int role)
{
	if (!index.isValid() || !index.internalPointer()) {
		return false;
	}

	GraphicalPartModelItem const * const item = static_cast<GraphicalPartModelItem const *>(index.internalPointer());
	switch (role) {
	case positionRole:
		mRepoApi.setGraphicalPartProperty(item->id(), item->index(), "position", value);
		break;
	case configurationRole:
		mRepoApi.setGraphicalPartProperty(item->id(), item->index(), "configuration", value);
		break;
	default:
		return false;
	}

	emit dataChanged(index, index);
	return true;
}

int GraphicalPartModel::rowCount(const QModelIndex &parent) const
{
	if (!parent.isValid()) {
		return mItems.size();
	}

	return mItems.at(parent.row()).size();
}

int GraphicalPartModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent)
	return 1;
}

QModelIndex GraphicalPartModel::index(int row, int column, QModelIndex const &parent) const
{
	if (column >= columnCount(parent) || row >= rowCount(parent)) {
		return QModelIndex();
	}

	if (!parent.isValid()) {
		return createIndex(row, column, static_cast<void *>(NULL));
	}

	QModelIndex const grandParent = parent.parent();
	if (grandParent.isValid()) {
		return QModelIndex();
	}

	return createIndex(row, column, mItems[parent.row()].at(row));
}

QModelIndex GraphicalPartModel::parent(QModelIndex const &index) const
{
	if (!index.isValid()) {
		return QModelIndex();
	}

	if (!index.internalPointer()) {
		return QModelIndex();
	}

	GraphicalPartModelItem const * const item = static_cast<GraphicalPartModelItem const *>(index.internalPointer());
	int const row = mIdPositions[item->id()];
	return createIndex(row, 0, static_cast<void *>(NULL));
}

QModelIndex GraphicalPartModel::addGraphicalPart(Id const &element, int index)
{
	int const parentRow = mIdPositions.contains(element) ? mIdPositions[element] : mItems.size();
	if (!mIdPositions.contains(element)) {
		beginInsertRows(QModelIndex(), parentRow, parentRow);
		mIdPositions.insert(element, parentRow);
		mItems.append(QList<modelsImplementation::GraphicalPartModelItem *>());
		endInsertRows();
	}

	QModelIndex const parentIndex = this->index(parentRow, 0, QModelIndex());
	int const row = mItems[parentRow].size();

	beginInsertRows(parentIndex, row, row);
	mItems[parentRow].append(new modelsImplementation::GraphicalPartModelItem(element, index));
	mRepoApi.createGraphicalPart(element, index);
	endInsertRows();

	return this->index(row, 0, parentIndex);
}

QModelIndex GraphicalPartModel::findIndex(Id const &element, int index)
{
	if (!mIdPositions.contains(element)) {
		return QModelIndex();
	}

	int const parentRow = mIdPositions.value(element);
	QModelIndex const parent = createIndex(parentRow, 0, static_cast<void *>(NULL));
	int partRow = 0;
	foreach (modelsImplementation::GraphicalPartModelItem const * const item, mItems[parentRow]) {
		if (item->index() == index) {
			break;
		}

		++partRow;
	}

	return this->index(partRow, 0, parent);
}
