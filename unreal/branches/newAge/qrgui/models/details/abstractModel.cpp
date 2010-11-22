#include <QtCore/QUuid>

#include "abstractModel.h"

using namespace qReal;
using namespace models;
using namespace details;

AbstractModel::AbstractModel(qrRepo::RepoApi &repoApi, const EditorManager &editorManager)
	: mApi(repoApi), mEditorManager(editorManager), mNotNeedUpdate(true)
{
}

Qt::ItemFlags AbstractModel::flags(QModelIndex const &index) const
{
	if (index.isValid()) {
		return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled
			| Qt::ItemIsDropEnabled | Qt::ItemIsEnabled;
	} else {
	// root item has invalid index, but we should still be able to drop elements into it
		return Qt::ItemIsDropEnabled;
	}
}

QVariant AbstractModel::data(const QModelIndex &index, int role) const
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
			case roles::positionRole:
				return mApi.property(item->id(), "position");
			case roles::fromRole:
				return mApi.from(item->id()).toVariant();
			case roles::toRole:
				return mApi.to(item->id()).toVariant();
			case roles::fromPortRole:
				return mApi.fromPort(item->id());
			case roles::toPortRole:
				return mApi.toPort(item->id());
			case roles::configurationRole:
				return mApi.property(item->id(), "configuration");
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

bool AbstractModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (index.isValid()) {
		AbstractModelItem *item = static_cast<AbstractModelItem *>(index.internalPointer());
		switch (role) {
		case Qt::DisplayRole:
		case Qt::EditRole:
			mApi.setName(item->id(), value.toString());
			break;
		case roles::positionRole:
			mApi.setProperty(item->id(), "position", value);
			break;
		case roles::configurationRole:
			mApi.setProperty(item->id(), "configuration", value);
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
		mNotNeedUpdate = false;
		emit dataChanged(index, index);
		return true;
	}
	return false;
}

QVariant AbstractModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 0)
		return QVariant("name");
	else
		return QVariant();
}

int AbstractModel::rowCount(const QModelIndex &parent) const
{
	return parentTreeItem(parent)->children().size();
}

int AbstractModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return 1;
}

QModelIndex AbstractModel::index(int row, int column, const QModelIndex &parent) const
{
	AbstractModelItem *parentItem = parentTreeItem(parent);
	if (parentItem->children().size() <= row)
		return QModelIndex();
	AbstractModelItem *item = parentItem->children().at(row);
	return createIndex(row, column, item);
}

QPersistentModelIndex AbstractModel::rootIndex() const
{
	return index(mRootItem);
}

AbstractModelItem *AbstractModel::parentTreeItem(QModelIndex const &parent) const
{
	return parent.isValid()
		? static_cast<AbstractModelItem*>(parent.internalPointer())
		: mRootItem
	;
}

QModelIndex AbstractModel::parent(const QModelIndex &index) const
{
	if (index.isValid()) {
		AbstractModelItem *item = static_cast<AbstractModelItem *>(index.internalPointer());
		AbstractModelItem *parentItem = item->parent();
		if (parentItem == mRootItem || parentItem == NULL)
			return QModelIndex();
		else
			return createIndex(parentItem->row(), 0, parentItem);
	} else
		return QModelIndex();
}

QModelIndex AbstractModel::index(AbstractModelItem const * const item) const
{
	QList<int> rowCoords;

	for (AbstractModelItem const *curItem = item;
		curItem != mRootItem; curItem = curItem->parent())
	{
		rowCoords.append(const_cast<AbstractModelItem *>(curItem)->row());
	}

	QModelIndex result;

	for (int i = rowCoords.size() - 1; i >= 0; --i) {
		result = index(rowCoords[i], 0, result);
	}

	return result;
}

QString AbstractModel::findPropertyName(Id const &id, int const role) const
{
	//In case of a property described in element itself (in metamodel),
	// role is simply an index of a property in a list of propertires.
	// This convention must be obeyed everywhere, otherwise roles will shift.
	QStringList properties = mEditorManager.getPropertyNames(id.type());
	Q_ASSERT(role - roles::customPropertiesBeginRole < properties.count());
	return properties[role - roles::customPropertiesBeginRole];
}

bool AbstractModel::dropMimeData(QMimeData const *data, Qt::DropAction action, int row, int column, QModelIndex const &parent)
{
	Q_UNUSED(row)
	Q_UNUSED(column)
	if (action == Qt::IgnoreAction)
		return true;
	else {
		AbstractModelItem *parentItem = parentTreeItem(parent);

		QByteArray dragData = data->data(DEFAULT_MIME_TYPE);

		QDataStream stream(&dragData, QIODevice::ReadOnly);
		QString idString;
		QString pathToItem;
		QString name;
		QPointF position;
		stream >> pathToItem;
		stream >> idString;
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

void AbstractModel::initializeElement(const Id &id, details::AbstractModelItem *parentItem,
		details::AbstractModelItem *item, const QString &name, const QPointF &position)
{
	parentItem->addChild(item);
	mApi.addChild(parentItem->id(), id);
	mApi.setProperty(id, "name", name);
	mApi.setProperty(id, "from", Id::rootId().toVariant());
	mApi.setProperty(id, "to", Id::rootId().toVariant());
	mApi.setProperty(id, "fromPort", 0.0);
	mApi.setProperty(id, "toPort", 0.0);
	mApi.setProperty(id, "links", IdListHelper::toVariant(IdList()));
	mApi.setProperty(id, "outgoingConnections", IdListHelper::toVariant(IdList()));
	mApi.setProperty(id, "incomingConnections", IdListHelper::toVariant(IdList()));
	mApi.setProperty(id, "outgoingUsages", IdListHelper::toVariant(IdList()));
	mApi.setProperty(id, "incomingUsages", IdListHelper::toVariant(IdList()));
	mApi.setProperty(id, "position", position);
	mApi.setProperty(id, "configuration", QVariant(QPolygon()));
	mModelItems.insert(id, item);
	endInsertRows();
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



