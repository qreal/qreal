#include "graphicalModel.h"

#include <QtCore/QUuid>

#include "models/details/logicalModel.h"

using namespace qReal;
using namespace models;
using namespace models::details;
using namespace modelsImplementation;

GraphicalModel::GraphicalModel(qrRepo::GraphicalRepoApi *repoApi
		, const EditorManagerInterface &editorManagerInterface
		)
	: AbstractModel(editorManagerInterface)
	, mLogicalModelView(this)
	, mApi(*repoApi)
	, mGraphicalAssistApi(nullptr)
{
	mRootItem = new GraphicalModelItem(Id::rootId(), Id(), nullptr);
	init();
}

GraphicalModel::~GraphicalModel()
{
	delete mGraphicalAssistApi;
	cleanupTree(mRootItem);
}

void GraphicalModel::setAssistApi(GraphicalModelAssistApi * const graphicalAssistApi)
{
	mGraphicalAssistApi = graphicalAssistApi;
}

void GraphicalModel::init()
{
	mModelItems.insert(Id::rootId(), mRootItem);
	mApi.setName(Id::rootId(), Id::rootId().toString());
	// Turn off view notification while loading. Model can be inconsistent during a process,
	// so views shall not update themselves before time. It is important for
	// scene, where adding edge before adding nodes may lead to disconnected edge.	blockSignals(true);
	blockSignals(true);
	loadSubtreeFromClient(static_cast<GraphicalModelItem *>(mRootItem));
	blockSignals(false);
}

void GraphicalModel::loadSubtreeFromClient(GraphicalModelItem * const parent)
{
	/// @todo There must be a way to tell nodes from edges in repository, or we need to implement correct loading
	/// at higher level. For now we check the existence of "from" property, which is bad, because a node can also
	/// have this property.
	/// Nodes need to be loaded before adges due to bugs in scene which connects edges to incorrect nodes or does
	/// not connect edges at all. Proper fix for that shall possibly be in scene instead of this place.
	foreach (const Id &childId, mApi.children(parent->id())) {
		if (mApi.isGraphicalElement(childId) && !mApi.hasProperty(childId, "from")) {
			GraphicalModelItem * const child = loadElement(parent, childId);
			loadSubtreeFromClient(child);
		}
	}

	foreach (const Id &childId, mApi.children(parent->id())) {
		if (mApi.isGraphicalElement(childId) && mApi.hasProperty(childId, "from")) {
			GraphicalModelItem * const child = loadElement(parent, childId);
			loadSubtreeFromClient(child);
		}
	}
}

GraphicalModelItem *GraphicalModel::loadElement(GraphicalModelItem *parentItem, const Id &id)
{
	int newRow = parentItem->children().size();

	beginInsertRows(index(parentItem), newRow, newRow);
	const Id logicalId = mApi.logicalId(id);
	GraphicalModelItem *item = new GraphicalModelItem(id, logicalId, parentItem);
	parentItem->addChild(item);
	mModelItems.insert(id, item);
	endInsertRows();

	return item;
}

void GraphicalModel::connectToLogicalModel(LogicalModel * const logicalModel)
{
	mLogicalModelView.setModel(logicalModel);
}

AbstractModelItem *GraphicalModel::createModelItem(const Id &id, AbstractModelItem *parentItem) const
{
	return new GraphicalModelItem(id, Id(id.type(), QUuid::createUuid().toString())
								  , static_cast<GraphicalModelItem *>(parentItem));
}

void GraphicalModel::updateElements(const Id &logicalId, const QString &name)
{
	foreach (AbstractModelItem *item,  mModelItems.values()) {
		GraphicalModelItem *graphicalItem = static_cast<GraphicalModelItem *>(item);
		if (graphicalItem->logicalId() == logicalId) {
			setNewName(graphicalItem->id(), name);
			emit dataChanged(index(graphicalItem), index(graphicalItem));
		}
	}
}

void GraphicalModel::addElementToModel(const Id &parent, const Id &id
		, const Id &logicalId, const QString &name, const QPointF &position)
{
	Q_ASSERT_X(mModelItems.contains(parent), "addElementToModel", "Adding element to non-existing parent");
	AbstractModelItem *parentItem = mModelItems[parent];

	GraphicalModelItem *newGraphicalModelItem = nullptr;
	Id actualLogicalId = logicalId;
	if (logicalId == Id::rootId() || logicalId.isNull()) {
		AbstractModelItem *newItem = createModelItem(id, parentItem);
		newGraphicalModelItem = static_cast<GraphicalModelItem *>(newItem);
		actualLogicalId = newGraphicalModelItem->logicalId();
	} else {
		GraphicalModelItem *graphicalParentItem = static_cast<GraphicalModelItem *>(parentItem);
		newGraphicalModelItem = new GraphicalModelItem(id, logicalId, graphicalParentItem);
	}

	initializeElement(id, actualLogicalId, parentItem, newGraphicalModelItem, name, position);
	emit elementAdded(id);
}

void GraphicalModel::initializeElement(const Id &id, const Id &logicalId
		, modelsImplementation::AbstractModelItem *parentItem, modelsImplementation::AbstractModelItem *item
		, const QString &name, const QPointF &position)
{
	const int newRow = parentItem->children().size();

	beginInsertRows(index(parentItem), newRow, newRow);
	parentItem->addChild(item);
	mApi.addChild(parentItem->id(), id, logicalId);
	mApi.setName(id, name);
	mApi.setFromPort(id, 0.0);
	mApi.setToPort(id, 0.0);
	mApi.setFrom(id, Id::rootId());
	mApi.setTo(id, Id::rootId());
	mApi.setProperty(id, "links", IdListHelper::toVariant(IdList()));
	mApi.setPosition(id, position);
	mApi.setConfiguration(id, QVariant(QPolygon()));
	mModelItems.insert(id, item);
	endInsertRows();
}

QVariant GraphicalModel::data(const QModelIndex &index, int role) const
{
	if (index.isValid()) {
		GraphicalModelItem *item = static_cast<GraphicalModelItem*>(index.internalPointer());
		Q_ASSERT(item);
		switch (role) {
		case Qt::DisplayRole:
		case Qt::EditRole:
			return mApi.name(item->id());
		case Qt::DecorationRole:
			return mEditorManagerInterface.icon(item->id());
		case roles::idRole:
			return item->id().toVariant();
		case roles::logicalIdRole:
			return item->logicalId().toVariant();
		case roles::positionRole:
			return mApi.position(item->id());
		case roles::fromRole:
			return mApi.from(item->id()).toVariant();
		case roles::toRole:
			return mApi.to(item->id()).toVariant();
		case roles::fromPortRole:
			return mApi.fromPort(item->id());
		case roles::toPortRole:
			return mApi.toPort(item->id());
		case roles::configurationRole:
			return mApi.configuration(item->id());
		}
		if (role >= roles::customPropertiesBeginRole) {
			return QVariant();  // Custom properties are invalid for graphical objects for now.
		}
		Q_ASSERT(role < Qt::UserRole);
		return QVariant();
	} else {
		return QVariant();
	}
}

bool GraphicalModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (index.isValid()) {
		AbstractModelItem *item = static_cast<AbstractModelItem *>(index.internalPointer());
		switch (role) {
		case Qt::DisplayRole:
		case Qt::EditRole:
			setNewName(item->id(), value.toString());
			break;
		case roles::positionRole:
			mApi.setPosition(item->id(), value);
			break;
		case roles::configurationRole:
			mApi.setConfiguration(item->id(), value);
			break;
		case roles::fromRole:
			mApi.setFrom(item->id(), value.value<Id>());
			break;
		case roles::toRole:
			mApi.setTo(item->id(), value.value<Id>());
			break;
		case roles::fromPortRole:
			mApi.setFromPort(item->id(), value.toDouble());
			break;
		case roles::toPortRole:
			mApi.setToPort(item->id(), value.toDouble());
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

void GraphicalModel::setNewName(const Id &id, const QString newValue)
{
	mApi.setName(id, newValue);
	emit nameChanged(id);
}

void GraphicalModel::changeParent(QModelIndex const &element, QModelIndex const &parent, const QPointF &position)
{
	if (!parent.isValid() || element.parent() == parent) {
		return;
	}

	int destinationRow = parentAbstractItem(parent)->children().size();

	if (beginMoveRows(element.parent(), element.row(), element.row(), parent, destinationRow)) {
		AbstractModelItem *elementItem = static_cast<AbstractModelItem*>(element.internalPointer());
		QVariant configuration = mApi.configuration(elementItem->id());
		elementItem->parent()->removeChild(elementItem);
		AbstractModelItem *parentItem = parentAbstractItem(parent);

		mApi.setParent(elementItem->id(), parentItem->id());

		elementItem->setParent(parentItem);

		parentItem->addChild(elementItem);

		mApi.setPosition(elementItem->id(), position);
		mApi.setConfiguration(elementItem->id(), configuration);
		endMoveRows();

		if (parent.row() != element.row()) {
			emit dataChanged(parent, element);
		}
	}
}

void GraphicalModel::stackBefore(const QModelIndex &element, const QModelIndex &sibling)
{
	if (element == sibling) {
		return;
	}

	if (beginMoveRows(element.parent(), element.row(), element.row(), element.parent(), sibling.row())) {
		AbstractModelItem *parent = static_cast<AbstractModelItem *>(element.parent().internalPointer());
		AbstractModelItem *item = static_cast<AbstractModelItem *>(element.internalPointer());
		AbstractModelItem *siblingItem = static_cast<AbstractModelItem *>(sibling.internalPointer());

		parent->stackBefore(item, siblingItem);
		mApi.stackBefore(parent->id(), item->id(), siblingItem->id());
		endMoveRows();
	} else {
		qDebug() << "GraphicalModel::stackBefore : problem with beginMoveRows";
	}
}

qrRepo::GraphicalRepoApi const &GraphicalModel::api() const
{
	return mApi;
}

qrRepo::GraphicalRepoApi &GraphicalModel::mutableApi() const
{
	return mApi;
}

GraphicalModelAssistApi &GraphicalModel::graphicalModelAssistApi() const
{
	return *mGraphicalAssistApi;
}

bool GraphicalModel::removeRows(int row, int count, QModelIndex const &parent)
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
			mApi.removeChild(parentItem->id(), child->id());
			mApi.removeElement(child->id());
			delete child;
			endRemoveRows();
		}
		return true;
	}
}

void GraphicalModel::removeModelItemFromApi(details::modelsImplementation::AbstractModelItem *const root
		, details::modelsImplementation::AbstractModelItem *child)
{
	mApi.removeProperty(child->id(), "position");
	mApi.removeProperty(child->id(), "configuration");
	if (mModelItems.count(child->id()) == 0) {
		mApi.removeChild(root->id(),child->id());
	}
	mApi.removeElement(child->id());
}

QList<QPersistentModelIndex> GraphicalModel::indexesWithLogicalId(const Id &logicalId) const
{
	QList<QPersistentModelIndex> indexes;
	foreach (AbstractModelItem *item, mModelItems.values()) {
		if (static_cast<GraphicalModelItem *>(item)->logicalId() == logicalId)
			indexes.append(index(item));
	}
	return indexes;
}

qReal::details::ModelsAssistInterface* GraphicalModel::modelAssistInterface() const
{
	return mGraphicalAssistApi;
}
