#include <QtGui>
#include <QtSql>

#include "realrepomodel.h"

RealRepoModel::RealRepoModel(QSqlDatabase db, QObject *parent)
	: QAbstractItemModel(parent)
{
	this->db = db;

	rootItem = new RepoTreeItem;
	rootItem->parent = 0;
	rootItem->id = 0;

	readRootTable();
}

RealRepoModel::~RealRepoModel()
{
	bytesCleaned = 0;
	cleanupTree(rootItem);
	qDebug() << bytesCleaned << "items cleaned,"
		<< sizeof(RepoTreeItem) << "bytes per tree item,"
		<< bytesCleaned * sizeof(RepoTreeItem) << "total";
}

QVariant RealRepoModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (index.column() != 0)
		return QVariant();

	RepoTreeItem *item = static_cast<RepoTreeItem *>(index.internalPointer());

	switch (role) {
		case Qt::DisplayRole:
		case Qt::EditRole:			return hashNames[item->id];
//		case Unreal::IdRole:		return item->id;
//		case Unreal::TypeRole:		return ???
		default:					return QVariant();
	}

	return QVariant();
}

bool RealRepoModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if (!index.isValid())
		return false;

	if (index.column() != 0)
		return false;

	RepoTreeItem *item = static_cast<RepoTreeItem*>(index.internalPointer());

	QSqlQuery q(db);

	switch (role) {
		case Qt::DisplayRole:
		case Qt::EditRole:				{
											q.prepare("UPDATE nametable SET name=:name WHERE id=:id ;");
											q.bindValue(":id",  item->id );
											q.bindValue(":name", value.toString() );
											if ( q.exec() ) {
												hashNames[item->id] = value.toString();
											} else {
												qDebug() << db.lastError().text();
											}
										}
										break;
		default:
										return false;
	}

	foreach(RepoTreeItem *item, hashTreeItems[item->id]) {
//		item->updateData();
		QModelIndex index = this->index(item);
		emit dataChanged(index,index);
	}

	return true;
}

Qt::ItemFlags RealRepoModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::ItemIsEnabled;

	RepoTreeItem *item = static_cast<RepoTreeItem*>(index.internalPointer());

	if ( isElement(item) )
		return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled; 
	else
		return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDropEnabled;
}

QVariant RealRepoModel::headerData(int section, Qt::Orientation orientation,
		int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 0 )
		return tr("Name");

	return QVariant();
}

QModelIndex RealRepoModel::index(const RepoTreeItem *item) const
{
	QList <int> rowCoords;

	for ( RepoTreeItem *curItem = const_cast<RepoTreeItem *>(item);
			curItem != rootItem; curItem = curItem->parent ) {
		rowCoords.append(curItem->row);
	}

	QModelIndex result;

	for ( int i = rowCoords.size() - 1; i >= 0; i-- ) {
		result = index(rowCoords[i],0,result);
	}

	return result;
}

QModelIndex RealRepoModel::index(int row, int column, const QModelIndex &parent)
	const
{
	RepoTreeItem *parentItem;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<RepoTreeItem*>(parent.internalPointer());

	if ( parentItem->children.isEmpty() ) {
		if (isElement(parentItem)) {
			const_cast<RealRepoModel *>(this)->readContainerTable(parentItem);
		} else {
			const_cast<RealRepoModel *>(this)->readCategoryTable(parentItem);
		}
	}

	RepoTreeItem *childItem = parentItem->children[row];

	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}

QModelIndex RealRepoModel::parent(const QModelIndex &child) const
{
	if (!child.isValid())
		return QModelIndex();

	RepoTreeItem *childItem = static_cast<RepoTreeItem*>(child.internalPointer());
	RepoTreeItem *parentItem = childItem->parent;

	if (!parentItem || parentItem == rootItem)
		return QModelIndex();

	return createIndex(parentItem->row, 0, parentItem);
}

int RealRepoModel::rowCount(const QModelIndex &parent) const
{
	RepoTreeItem *parentItem;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<RepoTreeItem*>(parent.internalPointer());

	return hashChildCount[parentItem->id];
}

int RealRepoModel::columnCount(const QModelIndex &/*parent*/) const
{
	return 1;
}

bool RealRepoModel::removeRows ( int row, int count, const QModelIndex & parent )
{
	RepoTreeItem *item;

	if ( parent.isValid() )
		item = static_cast<RepoTreeItem*>(parent.internalPointer());
	else {
		item = rootItem;
		return false;
	}

	beginRemoveRows(parent,row,row+count-1);

	QSqlQuery q(db);

	q.prepare("DELETE FROM nametable WHERE id=:id ;"
				"DELETE FROM diagram WHERE el_id=:id OR diagram_id=:id ;");

	for ( int i = row; i < row+count; i++ ) {
		qDebug() << "deleting row" << i << "with id" << item->children[i]->id << row << row+count;

		q.bindValue(":id",  item->children[i]->id );

		if ( !q.exec() )
			qDebug() << db.lastError().text();
	}

	for ( int i = row; i < row+count; i++ ) {
		hashTreeItems[item->children[row]->id].removeAll(item->children[row]);
		cleanupTree(item->children.at(row));

		item->children.removeAt(row);
		hashChildren[item->id].removeAt(row);
	}

	for ( int i = 0; i < item->children.size(); i++ ) {
		item->children[i]->row = i;
	}

	hashChildCount[item->id] -= count;

	endRemoveRows();

	return false;
}

QStringList RealRepoModel::mimeTypes () const
{
	QStringList types;
	types << "application/x-real-uml-data";
	return types;
}

Qt::DropActions RealRepoModel::supportedDropActions () const
{
	return Qt::CopyAction | Qt::MoveAction | Qt::LinkAction;
}

bool RealRepoModel::dropMimeData(const QMimeData *data, Qt::DropAction action,
		int row, int column, const QModelIndex &parent)
{
	if (action == Qt::IgnoreAction)
		return true;
	if (!data->hasFormat("application/x-real-uml-data"))
		return false;
	if (column > 0)
		return false;

	RepoTreeItem *item = static_cast<RepoTreeItem *>(parent.internalPointer());

	QString name;
	QByteArray dragData = data->data("application/x-real-uml-data");
	QDataStream stream(&dragData, QIODevice::ReadOnly);
	stream >> name;

	qDebug() << "dropMimeData" << item->id << action << name;

	return false;
}

void RealRepoModel::cleanupTree(RepoTreeItem *root)
{
	bytesCleaned ++;
	foreach (RepoTreeItem *childItem, root->children) {
		cleanupTree(childItem);
	}
	delete root;
}

void RealRepoModel::readRootTable()
{
	QSqlQuery q(db);
	if ( q.exec( "SELECT metatable.id, metatable.name, metatable.qualifiedName, COUNT(nametable.id)"
				"  FROM metatable LEFT JOIN nametable ON nametable.type=metatable.id"
				"  GROUP BY metatable.id;" ) ) {
		int i = 0;
		while ( q.next() ) {
			RepoTreeItem *item = new RepoTreeItem;
			item->parent = rootItem;
			item->row = i++;
			item->id = q.value(0).toInt();

			hashNames[item->id] = q.value(1).toString();
			hashChildCount[item->id] = q.value(3).toInt();
			hashTreeItems[item->id].append(item);

			rootItem->children.append(item);
		}

		hashChildCount[rootItem->id] = rootItem->children.size();
	} else
		qDebug() << db.lastError().text();
}

void RealRepoModel::readCategoryTable(RepoTreeItem *root)
{
	QSqlQuery q(db);
	q.prepare("SELECT nametable.id, nametable.name, nametable.type, nametable.qualifiedName, metatable.name, COUNT(diagram.diagram_id) FROM nametable"
			"  LEFT JOIN metatable ON metatable.id = nametable.type"
			"  LEFT JOIN diagram ON diagram.diagram_id = nametable.id"
			"  WHERE nametable.type = :type"
			"  GROUP BY diagram.diagram_id"
			"  ORDER BY diagram.diagram_id DESC ;");

	q.bindValue(0, root->id);

	if ( q.exec() ) {
		int i = 0;
		while ( q.next() ) {
			// 0 id
			// 1 name
			// 2 type
			// 3 description
			// 4 typeName
			// 5 childCount

			RepoTreeItem *item = new RepoTreeItem;
			item->parent = root;
			item->id = q.value(0).toInt();
			item->row = i++;
			root->children.append(item);
			hashTreeItems[item->id].append(item);

			// Some elements may be already added here - user may open diagrams before us.
			if ( ! hashNames.contains(item->id) ) {
				hashNames[item->id] = q.value(1).toString();
				hashTypes[item->id] = q.value(2).toInt();
				hashChildCount[item->id] = q.value(5).toInt();
				hashChildren[root->id].append(item->id);
			}
		}
	} else
		qDebug() << db.lastError().text();
}

void RealRepoModel::readContainerTable(RepoTreeItem *root)
{
	if ( hashChildren.contains(root->id) ) {
		int i = 0;
		foreach (int childId, hashChildren[root->id]) {
			RepoTreeItem *item = new RepoTreeItem;
			item->parent = root;
			item->row = i++;
			item->id = childId;

			root->children.append(item);
			hashTreeItems[item->id].append(item);
		}
	} else {
		QSqlQuery q(db);
		q.prepare("SELECT diagram.el_id, nametable.name, nametable.type, nametable.qualifiedName, COUNT (children.el_id) FROM diagram"
				"  LEFT JOIN nametable ON diagram.el_id = nametable.id"
				"  LEFT JOIN diagram AS children ON children.diagram_id = diagram.el_id"
				"  WHERE diagram.diagram_id = :type"
				"  GROUP BY children.diagram_id"
				"  ORDER BY diagram.el_id DESC ;");

		q.bindValue(0, root->id);

		if ( q.exec() ) {
			int i = 0;
			while ( q.next() ) {
				RepoTreeItem *item = new RepoTreeItem;
				item->parent = root;
				item->row = i++;
				item->id = q.value(0).toInt();

				hashNames[item->id] = q.value(1).toString();
				hashTypes[item->id] = q.value(2).toInt();

				hashChildCount[item->id] = q.value(4).toInt();

				hashChildren[root->id].append(item->id);

				root->children.append(item);
				hashTreeItems[item->id].append(item);
			}
		} else
			qDebug() << db.lastError().text();
	}
}
