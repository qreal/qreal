#include "model.h"
#include <QDebug>

using namespace qReal;

using namespace model;

Model::Model()
{
	mClient = new client::Client();
	rootItem = new ModelTreeItem(ROOT_ID,NULL);
	treeItems.insert(ROOT_ID,rootItem);
	mClient->setProperty(ROOT_ID,"Name",ROOT_ID.toString());
	loadSubtreeFromClient(rootItem);
}

Model::~Model()
{
	delete mClient;
}

Qt::ItemFlags Model::flags( const QModelIndex &index ) const
{
	if (index.isValid()) {
		return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled
			| Qt::ItemIsDropEnabled | Qt::ItemIsEnabled;
	} else {
		return Qt::NoItemFlags;
	}
}

QVariant Model::data( const QModelIndex &index, int role) const
{
	if (index.isValid()) {
		ModelTreeItem *item = static_cast<ModelTreeItem*>(index.internalPointer());
		switch (role) {
			case Qt::DisplayRole:
			case Qt::EditRole:
				return mClient->property(item->id(),"Name");
			case Qt::UserRole:
				return item->id().toString();
		}
		return QVariant();
	} else {
		return QVariant();
	}
}

QVariant Model::headerData( int section, Qt::Orientation orientation, int role ) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 0 ) {
		return QVariant("Name");
	} else {
		return QVariant();
	}
}

int Model::rowCount( const QModelIndex &parent ) const
{
	ModelTreeItem *parentItem;
	if (parent.isValid()) {
		parentItem = static_cast<ModelTreeItem*>(parent.internalPointer());
	} else {
		parentItem = rootItem;
	}
	return parentItem->children().size();
}

int Model::columnCount( const QModelIndex &parent ) const
{
	Q_UNUSED(parent)
	return 1;
}

bool Model::setData( const QModelIndex &index, const QVariant &value, int role )
{
	if (index.isValid()) {
		ModelTreeItem *item = static_cast<ModelTreeItem*>(index.internalPointer());
		switch (role) {
			case Qt::DisplayRole:
			case Qt::EditRole:
				mClient->setProperty(item->id(),"Name",value);
				return true;
		}
		return false;
	} else {
		return false;
	}
}

bool Model::removeRows( int row, int count, const QModelIndex &parent )
{
	if (parent.isValid()) {
		ModelTreeItem *parentItem = static_cast<ModelTreeItem*>(parent.internalPointer());
		if (parentItem->children().size() < row + count) {
			return false;
		} else {
			for (int i = row; i < row + count; i++) {
				removeModelItems(parentItem->children().at(i));
			}
			return true;
		}
	} else {
		return false;
	}
}

PropertyName Model::pathToItem( ModelTreeItem *item ) const
{
	if (item != rootItem) {
		PropertyName path;
		do {
			item = item->parent();
			path = item->id().toString() + PATH_DIVIDER + path;
		} while (item!=rootItem);
		return path;
	}
	else
		return "qrm:/";
}

void Model::removeConfigurationInClient( ModelTreeItem *item )
{
	mClient->removeProperty(item->id(),"position + " + pathToItem(item));
	mClient->removeProperty(item->id(),"configuration + " + pathToItem(item));
}

QModelIndex Model::index( ModelTreeItem *item )
{
	if (item!=rootItem) {
		return createIndex(item->row(),0,item);
	} else {
		return QModelIndex();
	}
}

void Model::removeModelItems( ModelTreeItem *root )
{
	foreach (ModelTreeItem *child, root->children()) {
		removeModelItems(child);
		int childRow = child->row();
		beginRemoveRows(index(root),childRow,childRow);
		removeConfigurationInClient(child);
		child->parent()->removeChild(child);
		treeItems.remove(child->id(),child);
		if (treeItems.count(child->id())==0) {
			mClient->removeChild(root->id(),child->id());
		}
		delete child;
		endRemoveRows();
	}
}

QModelIndex Model::index( int row, int column, const QModelIndex &parent ) const
{
	ModelTreeItem *parentItem;
	if (parent.isValid()) {
		parentItem = static_cast<ModelTreeItem*>(parent.internalPointer());
	} else {
		parentItem = rootItem;
	}
	if (parentItem->children().size()<=row) {
		return QModelIndex();
	}
	ModelTreeItem *item = parentItem->children().at(row);
	return createIndex(row,column,item);
}

QModelIndex Model::parent( const QModelIndex &index ) const
{
	if (index.isValid()) {
		ModelTreeItem *item = static_cast<ModelTreeItem*>(index.internalPointer());
		ModelTreeItem *parentItem = item->parent();
		if ((parentItem==rootItem)||(parentItem==NULL)) {
			return QModelIndex();
		} else{
			return createIndex(parentItem->row(),0,parentItem);
		}
	} else {
		return QModelIndex();
	}
}

Qt::DropActions Model::supportedDropActions() const
{
	return Qt::CopyAction | Qt::MoveAction | Qt::LinkAction;
}

QStringList Model::mimeTypes() const
{
	QStringList types;
	types.append(DEFAULT_MIME_TYPE);
	return types;
}

QMimeData* Model::mimeData( const QModelIndexList &indexes ) const
{
	QByteArray data;
	QDataStream stream(&data, QIODevice::WriteOnly);
	foreach (QModelIndex index, indexes) {
		if (index.isValid()) {
			ModelTreeItem *item = static_cast<ModelTreeItem*>(index.internalPointer());
			stream << item->id().toString();
			stream << pathToItem(item);
			stream << mClient->property(item->id(),"Name");
			stream << mClient->property(item->id(),"position + " + pathToItem(item)).toPointF();
		} else {
			stream << ROOT_ID.toString();
			stream << QString();
			stream << ROOT_ID.toString();
			stream << QPointF();
		}
	}
	QMimeData *mimeData = new QMimeData();
	mimeData->setData(DEFAULT_MIME_TYPE, data);
	return mimeData;
}

bool Model::dropMimeData( const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent )
{
	Q_UNUSED(row)
	Q_UNUSED(column)
	if (action == Qt::IgnoreAction) {
		return true;
	} else {
		ModelTreeItem *parentItem;
		if (parent.isValid()) {
			parentItem = static_cast<ModelTreeItem*>(parent.internalPointer());
		} else {
			parentItem = rootItem;
		}
		QByteArray dragData = data->data(DEFAULT_MIME_TYPE);
		QDataStream stream(&dragData, QIODevice::ReadOnly);
		QString idString;
		PropertyName pathToItem;
		QString name;
		QPointF position;
		stream >> idString;
		stream >> pathToItem;
		stream >> name;
		stream >> position;
		IdType id = Id::loadFromString(idString);
		return addElementToModel(parentItem,id,pathToItem,name,position,action) != NULL;
	}
}

ModelTreeItem *Model::addElementToModel( ModelTreeItem *parentItem, const IdType &id,
		const PropertyName &oldPathToItem, const QString &name, const QPointF &position, Qt::DropAction action )
{
	Q_UNUSED(oldPathToItem)
	Q_UNUSED(action)

	int newRow = parentItem->children().size();
	beginInsertRows(index(parentItem),newRow,newRow);
		ModelTreeItem *item = new ModelTreeItem(id,parentItem);
		parentItem->addChild(item);
		treeItems.insert(id,item);
		mClient->addChild(parentItem->id(),id);
		mClient->setProperty(id,"Name",name);
		mClient->setProperty(id,"position + " + pathToItem(item),position);
	endInsertRows();
	return item;
}

void Model::loadSubtreeFromClient(ModelTreeItem * const parent)
{
	foreach (IdType childId, mClient->children(parent->id())) {
		PropertyName path = pathToItem(parent);
		ModelTreeItem * child = addElementToModel(parent, childId, "",
			mClient->property(childId, "Name").toString(),
			mClient->property(childId, "position + " + path + PATH_DIVIDER).toPointF(),
			Qt::MoveAction);
		loadSubtreeFromClient(child);
	}
}

QPersistentModelIndex Model::rootIndex()
{
	return index(rootItem);
}