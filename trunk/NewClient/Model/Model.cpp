#include "Model.h"

using namespace qReal;

using namespace model;

Model::Model()
{
	mClient = new client::Client();
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
		ModelTreeItem *item = static_cast<ModelTreeItem*>(parent.internalPointer());	
		if (item->children().size() < row + count) {
			return false;
		} else {
			ModelTreeItemPointerList children = item->children();
			for (int i = row; i< row + count; i++) {
				removeModelItems(children.at(i));
				item->removeChild(children.at(i));
			}
			return true;
		}
	} else {
		return false;
	}
}

void Model::removeModelItems( ModelTreeItem *root )
{
// рекурсивное удаление
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
	// унылый рекурсивный поиск родителей до корня.
		return QModelIndex();
	} else {
		return QModelIndex();
	}
}