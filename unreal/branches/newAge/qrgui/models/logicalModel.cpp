#include "logicalModel.h"
#include "graphicalModel.h"

#include <QtCore/QUuid>

using namespace qReal;
using namespace models;
using namespace details;

LogicalModel::LogicalModel(qrRepo::LogicalRepoApi *repoApi, EditorManager const &editorManager)
	: AbstractModel(editorManager), mGraphicalModelView(this), mApi(*repoApi)
{
	mRootItem = new LogicalModelItem(Id::rootId(), NULL);
	mModelItems.insert(Id::rootId(), mRootItem);
}

void LogicalModel::connectToGraphicalModel(GraphicalModel * const graphicalModel)
{
	mGraphicalModelView.setModel(graphicalModel);
}

AbstractModelItem *LogicalModel::createModelItem(Id const &id, AbstractModelItem *parentItem) const
{
	return new LogicalModelItem(id, static_cast<LogicalModelItem *>(parentItem));
}

QModelIndex LogicalModel::indexById(Id const &id) const
{
	if (mModelItems.keys().contains(id)) {
		return index(mModelItems.find(id).value());
	}
	return QModelIndex();
}

void LogicalModel::updateElements(Id const &logicalId, QString const &name)
{
	if (!mNotNeedUpdate) {
		mNotNeedUpdate = true;
		return;
	}
	mApi.setName(logicalId, name);
	emit dataChanged(indexById(logicalId), indexById(logicalId));
}

QMimeData* LogicalModel::mimeData(QModelIndexList const &indexes) const
{
	QByteArray data;
	QDataStream stream(&data, QIODevice::WriteOnly);
	foreach (QModelIndex index, indexes) {
		if (index.isValid()) {
			AbstractModelItem *item = static_cast<AbstractModelItem*>(index.internalPointer());
			stream << item->id().toString();
			stream << pathToItem(item);
			stream << mApi.property(item->id(), "name").toString();
			stream << mApi.property(item->id(), "position").toPointF();
		} else {
			stream << Id::rootId().toString();
			stream << QString();
			stream << QString();
			stream << QPointF();
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
	Q_UNUSED(logicalId);
	if (mModelItems.contains(id))
		return;
	Q_ASSERT_X(mModelItems.contains(parent), "addElementToModel", "Adding element to non-existing parent");
	AbstractModelItem *parentItem = mModelItems[parent];

	AbstractModelItem *newItem = createModelItem(id, parentItem);
	initializeElement(id, parentItem, newItem, name, position);
}

void LogicalModel::initializeElement(const Id &id, details::AbstractModelItem *parentItem,
		details::AbstractModelItem *item, const QString &name, const QPointF &position)
{
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
		/*if (role >= roles::customPropertiesBeginRole) {
			QString selectedProperty = findPropertyName(item->id(), role);
			return mApi.property(item->id(), selectedProperty);
		}*/
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
			/*if (role >= roles::customPropertiesBeginRole) {
				QString selectedProperty = findPropertyName(item->id(), role);
				mApi.setProperty(item->id(), selectedProperty, value);
				break;
			}*/
			Q_ASSERT(role < Qt::UserRole);
			return false;
		}
		mNotNeedUpdate = false;
		emit dataChanged(index, index);
		return true;
	}
	return false;
}

void LogicalModel::changeParent(QModelIndex const &element, QModelIndex const &parent, QPointF const &position)
{
	if (!parent.isValid() || element.parent() == parent)
		return;

	int destinationRow = parentAbstractItem(parent)->children().size();

	if (beginMoveRows(element.parent(), element.row(), element.row(), parent, destinationRow)) {
		AbstractModelItem *elementItem = static_cast<AbstractModelItem*>(element.internalPointer());
		elementItem->parent()->removeChild(elementItem);
		AbstractModelItem *parentItem = parentAbstractItem(parent);

		mApi.addParent(elementItem->id(), parentItem->id());
		mApi.removeParent(elementItem->id(), elementItem->parent()->id());

		elementItem->setParent(parentItem);
		parentItem->addChild(elementItem);

		endMoveRows();
	}
}

bool LogicalModel::dropMimeData(QMimeData const *data, Qt::DropAction action, int row, int column, QModelIndex const &parent)
{
	Q_UNUSED(row)
	Q_UNUSED(column)
	if (action == Qt::IgnoreAction)
		return true;
	else {
		AbstractModelItem *parentItem = parentAbstractItem(parent);

		QByteArray dragData = data->data(DEFAULT_MIME_TYPE);

		QDataStream stream(&dragData, QIODevice::ReadOnly);
		QString idString;
		QString pathToItem;
		QString name;
		QPointF position;
		stream >> idString;
		stream >> pathToItem;
		stream >> name;
		stream >> position;

		Id logicalId = Id::rootId();
		Id id = Id::loadFromString(idString);
		if (mApi.exist(id)) {
			logicalId = id;
			Id newId = Id(id.editor(), id.diagram(), id.element(), QUuid::createUuid().toString());
			id = newId;
		}
		Q_ASSERT(id.idSize() == 4);

		if (mModelItems.contains(id))
			return false;

		addElementToModel(parentItem->id(), id, logicalId, name, position);
		return true;
	}
}
