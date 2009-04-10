#include "Model.h"

using namespace qReal;

using namespace model;

Model::Model()
{
	mClient = new client::Client();
	rootItem = new ModelTreeItem(ROOT_ID,NULL);
	treeItems.insert(ROOT_ID,rootItem);
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
	if (parent.isValid()) {
		ModelTreeItem *item = static_cast<ModelTreeItem*>(parent.internalPointer());
		return item->children().size();
	} else {
		return 0;
	}
}

int Model::columnCount( const QModelIndex &parent ) const
{
	if (parent.isValid()) {
		return 1;
	} else {
		return 0;
	}
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
	PropertyName path;
	do {
		item = item->parent();
		path = item->id() + PATH_DIVIDER + path;
	} while (item!=rootItem);
	return path;
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
	if (parent.isValid()) {
		ModelTreeItem *parentItem = static_cast<ModelTreeItem*>(parent.internalPointer());
		ModelTreeItem *item = parentItem->children().at(row);
		return createIndex(row,column,item);
	} else {
		return QModelIndex();
	}
}

QModelIndex Model::parent( const QModelIndex &index ) const
{
	if (index.isValid()) {
		ModelTreeItem *item = static_cast<ModelTreeItem*>(index.internalPointer());
		ModelTreeItem *parentItem = item->parent();
		if (parentItem==rootItem) {
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
			stream << item->id();
			stream << pathToItem(item);
			stream << mClient->property(item->id(),"position + " + pathToItem(item)).toPointF();
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
	if (parent.isValid()) {
		if (action == Qt::IgnoreAction) {
			return true;
		} else {
			ModelTreeItem *parentItem = static_cast<ModelTreeItem*>(parent.internalPointer());
			QByteArray dragData = data->data(DEFAULT_MIME_TYPE);
			QDataStream stream(&dragData, QIODevice::ReadOnly);
			IdType id;
			PropertyName pathToItem;
			QPointF position;
			stream >> id;
			stream >> pathToItem;
			stream >> position;
			return addElementToModel(parentItem,id,pathToItem,position,action);
		}
	} else {
		return false;
	}
}

bool Model::addElementToModel( ModelTreeItem *parentItem, const IdType &id,
		const PropertyName &pathToItem, const QPointF &position, Qt::DropAction action )
{
	ModelTreeItem *item = new ModelTreeItem(id,parentItem);
	parentItem->addChild(item);
	return true;
}

void Model::test()
{
	beginInsertRows(index(rootItem),0,0);
	ModelTreeItem *item1 = new ModelTreeItem("item1",rootItem);
	rootItem->addChild(item1);
	treeItems.insert("item1",item1);
	mClient->addChild(ROOT_ID,"item1");
	mClient->setProperty("item1","Name","anon");
	endInsertRows();
}
