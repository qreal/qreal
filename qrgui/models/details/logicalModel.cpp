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

#include "logicalModel.h"

#include <QtCore/QUuid>

#include <qrkernel/definitions.h>
#include <metaMetaModel/linkShape.h>

#include "models/details/graphicalModel.h"

using namespace qReal;
using namespace models;
using namespace models::details;
using namespace modelsImplementation;

LogicalModel::LogicalModel(qrRepo::LogicalRepoApi *repoApi, const EditorManagerInterface &editorManagerInterface)
		: AbstractModel(editorManagerInterface), mGraphicalModelView(this), mApi(*repoApi)
{
	mRootItem = new LogicalModelItem(Id::rootId(), nullptr);
	init();
	mLogicalAssistApi = new LogicalModelAssistApi(*this, editorManagerInterface);
}

LogicalModel::~LogicalModel()
{
	delete mLogicalAssistApi;
	cleanupTree(mRootItem);
}

void LogicalModel::init()
{
	mModelItems.insert(Id::rootId(), mRootItem);
	mApi.setName(Id::rootId(), Id::rootId().toString());
	// Turn off view notification while loading.
	blockSignals(true);
	loadSubtreeFromClient(static_cast<LogicalModelItem *>(mRootItem));
	blockSignals(false);
}

void LogicalModel::loadSubtreeFromClient(LogicalModelItem * const parent)
{
	foreach (Id childId, mApi.children(parent->id())) {
		if (mApi.isLogicalElement(childId)) {
			LogicalModelItem *child = loadElement(parent, childId);
			loadSubtreeFromClient(child);
		}
	}
}

LogicalModelItem *LogicalModel::loadElement(LogicalModelItem *parentItem, const Id &id)
{
	const int newRow = parentItem->children().size();

	beginInsertRows(index(parentItem), newRow, newRow);
	LogicalModelItem *item = new LogicalModelItem(id, parentItem);
	addInsufficientProperties(id);
	parentItem->addChild(item);
	mModelItems.insert(id, item);
	endInsertRows();

	return item;
}

void LogicalModel::addInsufficientProperties(const Id &id, const QString &name)
{
	if (!mEditorManagerInterface.hasElement(id.type())) {
		return;
	}

	QMap<QString, QVariant> standardProperties;
	standardProperties.insert("name", name);
	standardProperties.insert("from", Id::rootId().toVariant());
	standardProperties.insert("to", Id::rootId().toVariant());
	standardProperties.insert("linkShape", static_cast<int>(LinkShape::broken));
	standardProperties.insert("links", IdListHelper::toVariant(IdList()));
	standardProperties.insert("outgoingExplosion", Id().toVariant());
	standardProperties.insert("incomingExplosions", IdListHelper::toVariant(IdList()));
	foreach (const QString &property, standardProperties.keys()) {
		if (!mApi.hasProperty(id, property)) {
			mApi.setProperty(id, property, standardProperties[property]);
		}
	}

	const QStringList properties = mEditorManagerInterface.propertyNames(id.type());
	foreach (const QString &property, properties) {
		// for those properties that doesn't have default values, plugin will return empty string
		if (!mApi.hasProperty(id, property)) {
			mApi.setProperty(id, property, mEditorManagerInterface.defaultPropertyValue(id, property));
		}
	}
}

void LogicalModel::connectToGraphicalModel(GraphicalModel * const graphicalModel)
{
	mGraphicalModelView.setModel(graphicalModel);
}

AbstractModelItem *LogicalModel::createModelItem(const Id &id, AbstractModelItem *parentItem) const
{
	return new LogicalModelItem(id, static_cast<LogicalModelItem *>(parentItem));
}

void LogicalModel::updateElements(const Id &logicalId, const QString &name)
{
	if ((logicalId.isNull()) || (mApi.name(logicalId) == name)) {
		return;
	}
	mApi.setName(logicalId, name);
	emit dataChanged(indexById(logicalId), indexById(logicalId));
}

QMimeData* LogicalModel::mimeData(const QModelIndexList &indexes) const
{
	QByteArray data;
	QDataStream stream(&data, QIODevice::WriteOnly);
	for (const QModelIndex &index : indexes) {
		if (index.isValid()) {
			AbstractModelItem *item = static_cast<AbstractModelItem*>(index.internalPointer());
			const Id id = item->id();
			const bool isEdge = mLogicalAssistApi->editorManagerInterface().isNodeOrEdge(
					id.editor(), id.element()) == -1;

			stream << ElementInfo(id, id, mApi.property(id, "name").toString(), mApi.outgoingExplosion(item->id())
					, isEdge);
		} else {
			stream << ElementInfo();
		}
	}

	QMimeData *mimeData = new QMimeData();
	mimeData->setData(DEFAULT_MIME_TYPE, data);
	return mimeData;
}

void LogicalModel::addElementToModel(ElementInfo &elementInfo)
{
	if (mModelItems.contains(elementInfo.id())) {
		return;
	}

	Q_ASSERT_X(mModelItems.contains(elementInfo.logicalParent()), Q_FUNC_INFO, "Adding element to non-existing parent");
	AbstractModelItem * const parentItem = mModelItems[elementInfo.logicalParent()];
	AbstractModelItem * const newItem = createElementWithoutCommit(elementInfo, parentItem);
	if (!newItem) {
		return;
	}

	const int newRow = parentItem->children().size();
	beginInsertRows(index(parentItem), newRow, newRow);
	initializeElement(elementInfo, parentItem, newItem);

	endInsertRows();
	emit elementAdded(elementInfo.id());
}

/// @todo: unify this with graphical model
void LogicalModel::addElementsToModel(QList<ElementInfo> &elementsInfo)
{
	IdList parentsOrder;
	IdList edgesParentsOrder;
	QSet<Id> visitedElements;
	QMultiMap<Id, ElementInfo> parentsToChildrenMap;
	QMultiMap<Id, ElementInfo> parentsToEdgesMap;
	for (ElementInfo &elementInfo : elementsInfo) {
		if (elementInfo.isEdge()) {
			edgesParentsOrder << elementInfo.logicalParent();
			parentsToEdgesMap.insertMulti(elementInfo.logicalParent(), elementInfo);
		} else {
			parentsOrder << elementInfo.logicalParent();
			parentsToChildrenMap.insertMulti(elementInfo.logicalParent(), elementInfo);
		}
	}

	for (const Id &parent : parentsOrder) {
		if (!visitedElements.contains(parent)) {
			addTree(parent, parentsToChildrenMap, visitedElements);
		}
	}

	visitedElements.clear();
	for (const Id &parent : edgesParentsOrder) {
		addTree(parent, parentsToEdgesMap, visitedElements);
	}
}

void LogicalModel::addTree(const Id &parent, const QMultiMap<Id, ElementInfo> &childrenOfParents, QSet<Id> &visited)
{
	Q_ASSERT_X(mModelItems.contains(parent), Q_FUNC_INFO, "Adding element to non-existing parent");
	AbstractModelItem * const parentItem = mModelItems[parent];

	visited.insert(parent);
	QList<ElementInfo> children = childrenOfParents.values(parent);
	const int newRow = parentItem->children().size();
	beginInsertRows(index(parentItem), newRow, newRow + children.size() - 1);
	for (ElementInfo &child : children) {
		AbstractModelItem * const newGraphicalModelItem = createElementWithoutCommit(child, parentItem);
		initializeElement(child, parentItem, newGraphicalModelItem);
	}

	endInsertRows();

	for (const ElementInfo &child : children) {
		emit elementAdded(child.id());
		addTree(child.id(), childrenOfParents, visited);
	}
}

AbstractModelItem *LogicalModel::createElementWithoutCommit(const ElementInfo &elementInfo
		, AbstractModelItem *parentItem)
{
	AbstractModelItem *result = nullptr;

	if (elementInfo.logicalId() != Id::rootId() && mModelItems.contains(elementInfo.logicalId())) {
		if (elementInfo.logicalParent() == elementInfo.logicalId()) {
			return nullptr;
		}

		changeParent(index(mModelItems[elementInfo.logicalId()]), index(parentItem), QPointF());
	} else {
		result = createModelItem(elementInfo.id(), parentItem);
	}

	return result;
}

void LogicalModel::initializeElement(const ElementInfo &elementInfo
		, modelsImplementation::AbstractModelItem *parentItem
		, modelsImplementation::AbstractModelItem *item)
{
	parentItem->addChild(item);
	mApi.addChild(parentItem->id(), elementInfo.id());
	mApi.setMetaInformation(elementInfo.id().editor() + "Version"
			, mEditorManagerInterface.version(Id(elementInfo.id().editor())).toString());

	addInsufficientProperties(elementInfo.id(), elementInfo.name());
	for (const QString &property : elementInfo.logicalProperties()) {
		mApi.setProperty(elementInfo.id(), property, elementInfo.logicalProperty(property));
	}

	mModelItems.insert(elementInfo.id(), item);
}

QVariant LogicalModel::data(const QModelIndex &index, int role) const
{
	if (index.isValid()) {
		AbstractModelItem *item = static_cast<AbstractModelItem*>(index.internalPointer());
		Q_ASSERT(item);
		switch (role) {
			case Qt::DisplayRole:
			case Qt::EditRole:
				return mApi.name(item->id());
			case Qt::DecorationRole:
				return QVariant();
				// return mEditorManager.icon(item->id());
			case roles::idRole:
				return item->id().toVariant();
			case roles::fromRole:
				return mApi.from(item->id()).toVariant();
			case roles::toRole:
				return mApi.to(item->id()).toVariant();
		}
		if (role >= roles::customPropertiesBeginRole) {
			QString selectedProperty = findPropertyName(item->id(), role);
			return mApi.property(item->id(), selectedProperty);
		}
		Q_ASSERT(role < Qt::UserRole);
		return QVariant();
	} else {
		return QVariant();
	}
}

bool LogicalModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (index.isValid()) {
		AbstractModelItem *item = static_cast<AbstractModelItem *>(index.internalPointer());
		switch (role) {
		case Qt::DisplayRole:
		case Qt::EditRole:
			mApi.setName(item->id(), value.toString());
			break;
		case roles::fromRole:
			mApi.setFrom(item->id(), value.value<Id>());
			break;
		case roles::toRole:
			mApi.setTo(item->id(), value.value<Id>());
			break;
		default:
			if (role >= roles::customPropertiesBeginRole) {
				QString selectedProperty = findPropertyName(item->id(), role);
				mApi.setProperty(item->id(), selectedProperty, value);
				break;
			}
			Q_ASSERT(role < Qt::UserRole);
			return false;
		}
		emit dataChanged(index, index);
		return true;
	}
	return false;
}

void LogicalModel::changeParent(const QModelIndex &element, const QModelIndex &parent, const QPointF &position)
{
	Q_UNUSED(position)
	if (!parent.isValid() || element.parent() == parent) {
		return;
	}

	int destinationRow = parentAbstractItem(parent)->children().size();

	if (beginMoveRows(element.parent(), element.row(), element.row(), parent, destinationRow)) {
		AbstractModelItem *elementItem = static_cast<AbstractModelItem*>(element.internalPointer());
		elementItem->parent()->removeChild(elementItem);
		AbstractModelItem *parentItem = parentAbstractItem(parent);

		mApi.setParent(elementItem->id(), parentItem->id());

		elementItem->setParent(parentItem);

		parentItem->addChild(elementItem);

		endMoveRows();
	}
}

void LogicalModel::changeParent(const Id &parentId, const Id &childId)
{
	QModelIndex parentIndex = mLogicalAssistApi->indexById(parentId);
	QModelIndex childIndex = mLogicalAssistApi->indexById(childId);
	changeParent(childIndex, parentIndex, QPointF());
}

void LogicalModel::stackBefore(const QModelIndex &element, const QModelIndex &sibling)
{
	if (element == sibling) {
		return;
	}

	beginMoveRows(element.parent(), element.row(), element.row(), element.parent(), sibling.row());

	AbstractModelItem *parent = static_cast<AbstractModelItem *>(element.parent().internalPointer())
			, *item = static_cast<AbstractModelItem *>(element.internalPointer())
			, *siblingItem = static_cast<AbstractModelItem *>(sibling.internalPointer());

	parent->stackBefore(item, siblingItem);
	mApi.stackBefore(parent->id(), item->id(), siblingItem->id());
	endMoveRows();
}

const qrRepo::LogicalRepoApi &LogicalModel::api() const
{
	return mApi;
}

qrRepo::LogicalRepoApi &LogicalModel::mutableApi() const
{
	return mApi;
}

LogicalModelAssistApi &LogicalModel::logicalModelAssistApi() const
{
	return *mLogicalAssistApi;
}

bool LogicalModel::removeRows(int row, int count, const QModelIndex &parent)
{
	AbstractModelItem *parentItem = parentAbstractItem(parent);
	if (parentItem->children().size() < row + count) {
		return false;
	} else {
		for (int i = row; i < row + count; ++i) {
			AbstractModelItem *child = parentItem->children().at(i);
			removeModelItems(child);

			int childRow = child->row();
			beginRemoveRows(parent, childRow, childRow);
			child->parent()->removeChild(child);
			mModelItems.remove(child->id());
			if (mModelItems.count(child->id()) == 0)
				mApi.removeChild(parentItem->id(), child->id());
			mApi.removeElement(child->id());
			delete child;
			endRemoveRows();
		}
		return true;
	}
}

void LogicalModel::removeModelItemFromApi(details::modelsImplementation::AbstractModelItem *const root
		, details::modelsImplementation::AbstractModelItem *child)
{
	if (mModelItems.count(child->id())==0) {
		mApi.removeChild(root->id(),child->id());
	}
	mApi.removeElement(child->id());
}

qReal::details::ModelsAssistInterface* LogicalModel::modelAssistInterface() const
{
	return mLogicalAssistApi;
}
