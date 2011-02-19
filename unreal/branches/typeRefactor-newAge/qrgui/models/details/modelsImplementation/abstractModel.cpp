#include <QtCore/QUuid>
#include <QtCore/QDebug>

#include "abstractModel.h"

using namespace qReal;
using namespace models::details::modelsImplementation;

AbstractModel::AbstractModel(const EditorManager &editorManager)
	: mEditorManager(editorManager)
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

QVariant AbstractModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 0)
		return QVariant("name");
	else
		return QVariant();
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
	if (parentItem->children().size() <= row)
		return QModelIndex();
	AbstractModelItem *item = parentItem->children().at(row);
	return createIndex(row, column, item);
}

QPersistentModelIndex AbstractModel::rootIndex() const
{
	return index(mRootItem);
}

AbstractModelItem *AbstractModel::parentAbstractItem(QModelIndex const &parent) const
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

QString AbstractModel::findPropertyName(NewType const &type, int const role) const
{
	//In case of a property described in element itself (in metamodel),
	// role is simply an index of a property in a list of propertires.
	// This convention must be obeyed everywhere, otherwise roles will shift.
	QStringList properties = mEditorManager.getPropertyNames(type);
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

EditorManager const &AbstractModel::editorManager() const
{
	return mEditorManager;
}

QModelIndex AbstractModel::indexById(Id const &id) const
{
	if (mModelItems.keys().contains(id)) {
		return index(mModelItems.find(id).value());
	}
	return QModelIndex();
}

Id AbstractModel::idByIndex(QModelIndex const &index) const
{
	AbstractModelItem *item = static_cast<AbstractModelItem*>(index.internalPointer());
	return mModelItems.key(item);
}

bool AbstractModel::dropMimeData(QMimeData const *data, Qt::DropAction action, int row, int column, QModelIndex const &parent)
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
		QString typeString;
		QString pathToItem;
		QString name;
		QPointF position;
		bool isFromLogicalModel = false;
		stream >> idString;
		stream >> typeString;
		stream >> pathToItem;
		stream >> name;
		stream >> position;
		stream >> isFromLogicalModel;

		Id id = Id::loadFromString(idString);
		NewType type = NewType::loadFromString(typeString);

		Q_ASSERT(type.typeSize() == 4);
		if (mModelItems.contains(id))
			modelAssistApi()->changeParent(id, parentItem->id());
		else
			modelAssistApi()->createElement(parentItem->id(), id, type, isFromLogicalModel, name, position);
		return true;
	}
}

void AbstractModel::reinit()
{
	cleanupTree(mRootItem);
	mModelItems.clear();
	delete mRootItem;
	mRootItem = createModelItem(ROOT_ID, NULL);
	reset();
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
