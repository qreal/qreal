/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "graphicalPartModel.h"

#include "models/details/graphicalModel.h"

using namespace qReal;
using namespace qReal::models::details;
using namespace qReal::models::details::modelsImplementation;

GraphicalPartModel::GraphicalPartModel(qrRepo::GraphicalRepoApi &repoApi
		, const modelsImplementation::ModelIndexesInterface &graphicalModel)
	: mRepoApi(repoApi)
	, mGraphicalModel(graphicalModel)
{
	load();
}

GraphicalPartModel::~GraphicalPartModel()
{
	clear();
}

void GraphicalPartModel::reinit()
{
	blockSignals(true);
	clear();
	mItems.clear();
	mIdPositions.clear();
	load();
	blockSignals(false);
}

QVariant GraphicalPartModel::data(const QModelIndex &index, int role) const
{
	const GraphicalPartModelItem * const item = static_cast<const GraphicalPartModelItem *>(index.internalPointer());
	switch (role) {
	case positionRole:
		return mRepoApi.graphicalPartProperty(item->id(), item->index(), "position");
	case configurationRole:
		return mRepoApi.graphicalPartProperty(item->id(), item->index(), "configuration");
	default:
		return QVariant();
	}
}

bool GraphicalPartModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (!index.isValid() || !index.internalPointer()) {
		return false;
	}

	const GraphicalPartModelItem * const item = static_cast<const GraphicalPartModelItem *>(index.internalPointer());
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

QModelIndex GraphicalPartModel::index(int row, int column, const QModelIndex &parent) const
{
	if (column >= columnCount(parent) || row >= rowCount(parent)) {
		return QModelIndex();
	}

	if (!parent.isValid()) {
		return createIndex(row, column, static_cast<void *>(nullptr));
	}

	const QModelIndex grandParent = parent.parent();
	if (grandParent.isValid()) {
		return QModelIndex();
	}

	return createIndex(row, column, mItems[parent.row()].at(row));
}

QModelIndex GraphicalPartModel::parent(const QModelIndex &index) const
{
	if (!index.isValid()) {
		return QModelIndex();
	}

	if (!index.internalPointer()) {
		return QModelIndex();
	}

	const GraphicalPartModelItem * const item = static_cast<const GraphicalPartModelItem *>(index.internalPointer());
	const int row = mIdPositions[item->id()];
	return createIndex(row, 0, static_cast<void *>(nullptr));
}

bool GraphicalPartModel::removeRows(int row, int count, const QModelIndex &parent)
{
	if (parent.isValid()) {
		// We shall not be able to remove one part now, only element as a whole.
		return false;
	}

	if (row + count > mItems.size()) {
		return false;
	}

	beginRemoveRows(parent, row, row + count - 1);
	mIdPositions.clear();
	for (int i = row; i < row + count; ++i) {
		qDeleteAll(mItems[i]);
		mItems.removeAt(i);
	}

	for (int i = 0; i < mItems.size(); ++i) {
		if (mItems[i].size() > 0) {
			mIdPositions[mItems[i][0]->id()] = i;
		}
	}

	endRemoveRows();

	return true;
}

QModelIndex GraphicalPartModel::addGraphicalPart(const Id &element, int index)
{
	return addGraphicalPart(element, index, true);
}

QModelIndex GraphicalPartModel::findIndex(const Id &element, int index) const
{
	if (!mIdPositions.contains(element)) {
		return QModelIndex();
	}

	const int parentRow = mIdPositions.value(element);
	const QModelIndex parent = createIndex(parentRow, 0, static_cast<void *>(nullptr));
	int partRow = 0;
	foreach (const modelsImplementation::GraphicalPartModelItem * const item, mItems[parentRow]) {
		if (item->index() == index) {
			break;
		}

		++partRow;
	}

	if (partRow == mItems[parentRow].size()) {
		return QModelIndex();
	}

	return this->index(partRow, 0, parent);
}

void GraphicalPartModel::rowsAboutToBeRemovedInGraphicalModel(const QModelIndex &parent, int start, int end)
{
	for (int row = start; row <= end; ++row) {
		const QModelIndex current = mGraphicalModel.index(row, 0, parent);
		if (current.isValid()) {
			const Id graphicalId = current.data(roles::idRole).value<Id>();
			if (!mIdPositions.contains(graphicalId)) {
				return;
			}

			removeRow(mIdPositions.value(graphicalId));
		}
	}
}

void GraphicalPartModel::clear()
{
	typedef QList<modelsImplementation::GraphicalPartModelItem *> GraphicalPartModelItemList;
	foreach (const GraphicalPartModelItemList &list, mItems) {
		qDeleteAll(list);
	}
}

void GraphicalPartModel::load(const Id &parent)
{
	loadElement(parent);
	foreach (const Id &child, mRepoApi.children(parent)) {
		if (mRepoApi.isGraphicalElement(child)) {
			load(child);
		}
	}
}

void GraphicalPartModel::loadElement(const Id &id)
{
	foreach (int partIndex, mRepoApi.graphicalParts(id)) {
		addGraphicalPart(id, partIndex, false);
	}
}

QModelIndex GraphicalPartModel::addGraphicalPart(const Id &element, int index, bool addToRepo)
{
	const int parentRow = mIdPositions.contains(element) ? mIdPositions[element] : mItems.size();
	if (!mIdPositions.contains(element)) {
		beginInsertRows(QModelIndex(), parentRow, parentRow);
		mIdPositions.insert(element, parentRow);
		mItems.append(QList<modelsImplementation::GraphicalPartModelItem *>());
		endInsertRows();
	}

	const QModelIndex parentIndex = this->index(parentRow, 0, QModelIndex());
	const int row = mItems[parentRow].size();

	beginInsertRows(parentIndex, row, row);
	mItems[parentRow].append(new modelsImplementation::GraphicalPartModelItem(element, index));
	if (addToRepo) {
		mRepoApi.createGraphicalPart(element, index);
	}
	endInsertRows();

	return this->index(row, 0, parentIndex);
}
