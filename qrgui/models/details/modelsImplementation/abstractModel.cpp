/* Copyright 2007-2016 QReal Research Group
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

#include "abstractModel.h"

#include <QtCore/QUuid>

#include <qrkernel/definitions.h>

using namespace qReal;
using namespace models::details::modelsImplementation;

AbstractModel::AbstractModel(const EditorManagerInterface &editorManagerInterface)
		: mEditorManagerInterface(editorManagerInterface)
{
}

Qt::ItemFlags AbstractModel::flags(const QModelIndex &index) const
{
	if (index.isValid()) {
		return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled
				| Qt::ItemIsDropEnabled | Qt::ItemIsEnabled;
	} else {
	// root item has invalid index, but we should still be able to drop elements into it
		return Qt::ItemIsDropEnabled;
	}
}

QVariant AbstractModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 0) {
		return QVariant(tr("name"));
	} else {
		return QVariant();
	}
}

int AbstractModel::rowCount(const QModelIndex &parent) const
{
	return parentAbstractItem(parent)->children().size();
}

int AbstractModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return 1;
}

QModelIndex AbstractModel::index(int row, int column, const QModelIndex &parent) const
{
	AbstractModelItem *parentItem = parentAbstractItem(parent);

	if (parentItem->children().size() <= row) {
		return QModelIndex();
	}

	AbstractModelItem *item = parentItem->children().at(row);
	return createIndex(row, column, item);
}

QPersistentModelIndex AbstractModel::rootIndex() const
{
	return index(mRootItem);
}

AbstractModelItem *AbstractModel::parentAbstractItem(const QModelIndex &parent) const
{
	return parent.isValid()
		? static_cast<AbstractModelItem*>(parent.internalPointer())
		: mRootItem;
}

QModelIndex AbstractModel::parent(const QModelIndex &index) const
{
	if (index.isValid()) {
		AbstractModelItem *item = static_cast<AbstractModelItem *>(index.internalPointer());
		AbstractModelItem *parentItem = item->parent();
		if (parentItem == mRootItem || parentItem == nullptr) {
			return QModelIndex();
		} else {
			return createIndex(parentItem->row(), 0, parentItem);
		}
	} else {
		return QModelIndex();
	}
}

QModelIndex AbstractModel::index(const AbstractModelItem * const item) const
{
	QList<int> rowCoords;

	for (const AbstractModelItem *curItem = item; curItem != mRootItem; curItem = curItem->parent()) {
		rowCoords.append(curItem->row());
	}

	QModelIndex result;

	for (int i = rowCoords.size() - 1; i >= 0; --i) {
		result = index(rowCoords[i], 0, result);
	}

	return result;
}

QString AbstractModel::findPropertyName(const Id &id, const int role) const
{
	// In case of a property described in element itself (in metamodel),
	// role is simply an index of a property in a list of properties.
	// This convention must be obeyed everywhere, otherwise roles will shift.
	QStringList properties = mEditorManagerInterface.propertyNames(id.type());
	Q_ASSERT(role - roles::customPropertiesBeginRole < properties.count());
	return properties[role - roles::customPropertiesBeginRole];
}

Qt::DropActions AbstractModel::supportedDropActions() const
{
	return Qt::CopyAction | Qt::MoveAction | Qt::LinkAction;
}

QStringList AbstractModel::mimeTypes() const
{
	QStringList types;
	types.append(DEFAULT_MIME_TYPE);
	return types;
}

const EditorManagerInterface &AbstractModel::editorManagerInterface() const
{
	return mEditorManagerInterface;
}

QModelIndex AbstractModel::indexById(const Id &id) const
{
	if (mModelItems.contains(id)) {
		return index(mModelItems.find(id).value());
	}

	return QModelIndex();
}

Id AbstractModel::idByIndex(const QModelIndex &index) const
{
	AbstractModelItem *item = static_cast<AbstractModelItem*>(index.internalPointer());
	return mModelItems.key(item);
}

Id AbstractModel::rootId() const
{
	return mRootItem->id();
}

bool AbstractModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row
		, int column, const QModelIndex &parent)
{
	Q_UNUSED(row)
	Q_UNUSED(column)

	if (action == Qt::IgnoreAction) {
		return true;
	}

	AbstractModelItem *parentItem = parentAbstractItem(parent);

	ElementInfo element = ElementInfo::fromMimeData(data);
	Q_ASSERT(element.id().idSize() == 4);

	if (mModelItems.contains(element.id())) {
		modelAssistInterface()->changeParent(element.id(), parentItem->id());
	} else {
		element.setGraphicalParent(parentItem->id());
		element.setLogicalParent(parentItem->id());
		modelAssistInterface()->createElements(QList<ElementInfo>() << element);
	}

	return true;
}

void AbstractModel::reinit()
{
	cleanupTree(mRootItem);
	mModelItems.clear();
	delete mRootItem;
	mRootItem = createModelItem(Id::rootId(), nullptr);
	beginResetModel();
	endResetModel();
	init();
}

void AbstractModel::cleanupTree(modelsImplementation::AbstractModelItem * item)
{
	foreach (AbstractModelItem *childItem, item->children()) {
		cleanupTree(childItem);
		delete childItem;
	}
	item->clearChildren();
}

void AbstractModel::removeModelItems(details::modelsImplementation::AbstractModelItem *const root)
{
	foreach (AbstractModelItem *child, root->children()) {
		removeModelItems(child);
		int childRow = child->row();
		beginRemoveRows(index(root),childRow,childRow);
		child->parent()->removeChild(child);
		mModelItems.remove(child->id());
		removeModelItemFromApi(root, child);
		delete child;
		endRemoveRows();
	}
}
