#include "logicalModel.h"
#include "graphicalModel.h"

#include <QtCore/QUuid>

using namespace qReal;
using namespace models;
using namespace models::details;
using namespace modelsImplementation;

LogicalModel::LogicalModel(qrRepo::LogicalRepoApi *repoApi, EditorManager const &editorManager)
	: AbstractModel(editorManager), mGraphicalModelView(this), mApi(*repoApi)
{
	mRootItem = new LogicalModelItem(Id::rootId(), NULL);
	init();
	mLogicalAssistApi = new LogicalModelAssistApi(*this, editorManager);
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

LogicalModelItem *LogicalModel::loadElement(LogicalModelItem *parentItem, Id const &id)
{
//	if (isDiagram(id)) {
//		mApi.addOpenedDiagram(id);
//	}

	int newRow = parentItem->children().size();

	beginInsertRows(index(parentItem), newRow, newRow);
	LogicalModelItem *item = new LogicalModelItem(id, parentItem);
	checkProperties(id);
	parentItem->addChild(item);
	mModelItems.insert(id, item);
	endInsertRows();

	return item;
}

void LogicalModel::checkProperties(Id const &id)
{
	if (!mEditorManager.hasElement(id.type()))
		return;
	QStringList const propertiesThatShallBe = mEditorManager.getPropertyNames(id.type());
	foreach (QString const property, propertiesThatShallBe)
		if (!api().hasProperty(id, property))
			mApi.setProperty(id, property, "");  // There shall be default value.
	if (!mApi.hasProperty(id, "outgoingUsages"))
		mApi.setProperty(id, "outgoingUsages", IdListHelper::toVariant(IdList()));
	if (!mApi.hasProperty(id, "incomingUsages"))
		mApi.setProperty(id, "incomingUsages", IdListHelper::toVariant(IdList()));
}

void LogicalModel::connectToGraphicalModel(GraphicalModel * const graphicalModel)
{
	mGraphicalModelView.setModel(graphicalModel);
}

AbstractModelItem *LogicalModel::createModelItem(Id const &id, AbstractModelItem *parentItem) const
{
	return new LogicalModelItem(id, static_cast<LogicalModelItem *>(parentItem));
}

void LogicalModel::updateElements(Id const &logicalId, QString const &name)
{
	if (mApi.name(logicalId) == name)
		return;
	mApi.setName(logicalId, name);
	emit dataChanged(indexById(logicalId), indexById(logicalId));
}

QMimeData* LogicalModel::mimeData(QModelIndexList const &indexes) const
{
	QByteArray data;
	bool isFromLogicalModel = true;
	QDataStream stream(&data, QIODevice::WriteOnly);
	foreach (QModelIndex index, indexes) {
		if (index.isValid()) {
			AbstractModelItem *item = static_cast<AbstractModelItem*>(index.internalPointer());
			stream << item->id().toString();
			stream << pathToItem(item);
			stream << mApi.property(item->id(), "name").toString();
			stream << QPointF();
			stream << isFromLogicalModel;
		} else {
			stream << Id::rootId().toString();
			stream << QString();
			stream << QString();
			stream << QPointF();
			stream << isFromLogicalModel;
		}
	}

	QMimeData *mimeData = new QMimeData();
	mimeData->setData(DEFAULT_MIME_TYPE, data);
	return mimeData;
}

QString LogicalModel::pathToItem(AbstractModelItem const *item) const
{
	if (item != mRootItem) {
		QString path;
		do {
			item = item->parent();
			path = item->id().toString() + ID_PATH_DIVIDER + path;
		} while (item != mRootItem);
		return path;
	}
	else
		return Id::rootId().toString();
}

void LogicalModel::addElementToModel(const Id &parent, const Id &id, const Id &logicalId, const QString &name, const QPointF &position)
{
	if (mModelItems.contains(id))
		return;
	Q_ASSERT_X(mModelItems.contains(parent), "addElementToModel", "Adding element to non-existing parent");
	AbstractModelItem *parentItem = mModelItems[parent];
	AbstractModelItem *newItem = NULL;
	if ((logicalId != Id::rootId()) && (mModelItems.contains(logicalId))) {
		 if (parent == logicalId)
			 return;
		else
			changeParent(index(mModelItems[logicalId]), index(parentItem), QPointF());
		}
	else {
		newItem = createModelItem(id, parentItem);
		initializeElement(id, parentItem, newItem, name, position);
	}
}

void LogicalModel::initializeElement(const Id &id, modelsImplementation::AbstractModelItem *parentItem,
		modelsImplementation::AbstractModelItem *item, const QString &name, const QPointF &position)
{
	Q_UNUSED(position)
	int const newRow = parentItem->children().size();
	beginInsertRows(index(parentItem), newRow, newRow);
	parentItem->addChild(item);
	mApi.addChild(parentItem->id(), id);
	mApi.setProperty(id, "name", name);
	mApi.setProperty(id, "from", Id::rootId().toVariant());
	mApi.setProperty(id, "to", Id::rootId().toVariant());
	mApi.setProperty(id, "links", IdListHelper::toVariant(IdList()));
	mApi.setProperty(id, "outgoingConnections", IdListHelper::toVariant(IdList()));
	mApi.setProperty(id, "incomingConnections", IdListHelper::toVariant(IdList()));
	mApi.setProperty(id, "outgoingUsages", IdListHelper::toVariant(IdList()));
	mApi.setProperty(id, "incomingUsages", IdListHelper::toVariant(IdList()));

	QStringList const properties = mEditorManager.getPropertyNames(id.type());
	foreach (QString const property, properties)
	// for those properties that doesn't have default values, plugin will return empty string
		mApi.setProperty(id, property, mEditorManager.getDefaultPropertyValue(id, property));

	mModelItems.insert(id, item);
	endInsertRows();
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

void LogicalModel::changeParent(QModelIndex const &element, QModelIndex const &parent, QPointF const &position)
{
	Q_UNUSED(position)
	if (!parent.isValid() || element.parent() == parent)
		return;

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

qrRepo::LogicalRepoApi const &LogicalModel::api() const
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

bool LogicalModel::removeRows(int row, int count, QModelIndex const &parent)
{
	AbstractModelItem *parentItem = parentAbstractItem(parent);
	if (parentItem->children().size() < row + count)
		return false;
	else {
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

void LogicalModel::removeModelItemFromApi(details::modelsImplementation::AbstractModelItem *const root, details::modelsImplementation::AbstractModelItem *child)
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
