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
	typedef QHash<int, modelsImplementation::GraphicalPartModelItem *> GraphicalPartModelItemHash;
	foreach (GraphicalPartModelItemHash const &hash, mItems.values()) {
		qDeleteAll(hash.values());
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

	return mItems.values().at(parent.row()).size();
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

	Id const id = mItems.keys().at(parent.row());
	return createIndex(row, column, mItems[id].values().at(row));
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
	int const row = mItems.keys().indexOf(item->id());
	return createIndex(row, 0, static_cast<void *>(NULL));
}

QModelIndex GraphicalPartModel::addGraphicalPart(Id const &element, int index)
{
	// TODO: beginInsertRows
	GraphicalPartModelItem * const item = new GraphicalPartModelItem(element, index);
	mItems[element][index] = item;
	mRepoApi.createGraphicalPart(element, index);

	return findIndex(element, index);
}

QModelIndex GraphicalPartModel::findIndex(Id const &element, int index)
{
	int const parentRow = mItems.keys().indexOf(element);
	QModelIndex const parent = createIndex(parentRow, 0, static_cast<void *>(NULL));
	int const partRow = mItems[element].keys().indexOf(index);
	return this->index(partRow, 0, parent);
}
