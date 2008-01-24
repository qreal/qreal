#include <QtGui>
#include <QtSql/QtSql>

#include "realrepomodel.h"
#include "realreporoles.h"

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
	cleanupTree(rootItem);
	delete rootItem;
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
		case Qt::EditRole:
		case Unreal::krnnNamedElement::nameRole:
			return hashNames[item->id];
		case Qt::DecorationRole:
			{
				if ( hashTypes.contains(item->id) )
					return info.objectIcon(hashTypes.value(item->id));
				else
					return info.objectIcon(item->id);
			};
		case Unreal::IdRole:
			return item->id;
		case Unreal::TypeRole:
			return hashTypes[item->id];
		case Unreal::PositionRole:
			{
				if ( type( item->parent ) == Container )
					if ( hashDiagramElements[item->parent->id].contains(item->id) )
						return hashDiagramElements[item->parent->id][item->id].position;

				return QVariant();
			};
		case Unreal::ConfigurationRole:
			{
				if ( type( item->parent ) == Container )
					if ( hashDiagramElements[item->parent->id].contains(item->id) )
						return hashDiagramElements[item->parent->id][item->id].configuration;

				return QVariant();
			};
		default:
			if ( role > Unreal::UserRole ) {
				if ( hashElementProps.contains(item->id) ) {
					if ( hashElementProps[item->id].contains(role) ){
						return hashElementProps[item->id][role];
					}    
					else
						return QVariant();
				} else {
					const_cast<RealRepoModel *>(this)->updateProperties(item->id);
					return hashElementProps[item->id][role];
				}
			} else {
				return QVariant(); //for now
			}
	}

	return QVariant();
}

bool RealRepoModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
//	qDebug() << __PRETTY_FUNCTION__;
	if (!index.isValid())
		return false;

	if (index.column() != 0)
		return false;

	RepoTreeItem *item = static_cast<RepoTreeItem*>(index.internalPointer());

	QSqlQuery q(db);

	switch (role) {
		case Qt::DisplayRole:
		case Unreal::krnnNamedElement::nameRole:
		case Qt::EditRole:
			{
				q.prepare("UPDATE nametable SET name=:name WHERE id=:id ;");
				q.bindValue(":id",  item->id );
				q.bindValue(":name", value.toString() );
				if ( q.exec() ) {
					hashNames[item->id] = value.toString();
				} else {
					qDebug() << q.executedQuery() << db.lastError().text();
				}
			}
			break;
		case Unreal::PositionRole:
			{
				if ( type(item->parent) == Container ) {
					q.prepare("UPDATE diagram SET x=:x , y=:y  WHERE diagram_id=:did AND el_id=:elid;");
					q.bindValue(":did", item->parent->id );
					q.bindValue(":elid", item->id );
					q.bindValue(":x", value.toPoint().x() );
					q.bindValue(":y", value.toPoint().y() );

					if ( q.exec() ) {
						hashDiagramElements[item->parent->id][item->id].position = value.toPoint();
					} else
						qDebug() << q.executedQuery() << db.lastError().text();
				}
			}
		case Unreal::ConfigurationRole:
			{
				if ( type(item->parent) == Container ) {
					q.prepare("UPDATE diagram SET cfg=:config WHERE diagram_id=:did AND el_id=:elid;");
					q.bindValue(":did", item->parent->id );
					q.bindValue(":elid", item->id );
					// save QPolygon to QString
					QPolygon poly(value.value<QPolygon>());
					QString result;
					foreach ( QPoint point, poly ) {
						result += QString("(%1,%2);").arg(point.x()).arg(point.y());
					}
					result.chop(1);

					q.bindValue(":config", result);

					if ( q.exec() ) {
						hashDiagramElements[item->parent->id][item->id].configuration = value.value<QPolygon>();
					} else
						qDebug() << q.executedQuery() << db.lastError().text();
				}
			}
		default:
			if ( role >= Unreal::UserRole ) {
				QString sql = QString("UPDATE el_%1 SET `%2` = :value WHERE id = :elid ;")
					.arg(hashTypes[item->id]).arg(info.getColumnName(hashTypes[item->id],role));
				q.prepare(sql);
				q.bindValue(":elid", item->id);
				q.bindValue(":value", value);

				if ( q.exec() ) {
					if ( hashElementProps.contains(item->id) ) {
						hashElementProps[item->id][role] = value;
					}
				} else
					qDebug() << q.executedQuery() << db.lastError().text();
			}
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
	switch ( type(index) ) {
		case Container:		return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable
							/* | Qt::ItemIsDragEnabled */ | Qt::ItemIsDropEnabled;
		case Category:		return Qt::ItemIsEnabled | Qt::ItemIsDropEnabled;
		case Root:			return Qt::ItemIsEnabled;
		default:			return 0;
	}
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
		if (type(parentItem) == Container ) {
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

	if (parentItem == rootItem)
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
	RepoTreeItem *parentItem;

	if ( parent.isValid() )
		parentItem = static_cast<RepoTreeItem*>(parent.internalPointer());
	else {
		return false;
	}

	if ( type( parentItem ) == Container ) {
		beginRemoveRows(parent,row,row+count-1);
		QSqlQuery q(db);
		q.prepare("DELETE FROM diagram WHERE el_id=:id AND diagram_id=:did ;");

			for ( int i = row; i < row+count; i++ ) {
				// qDebug() << "deleting row" << i << "with id" << parentItem->children[i]->id << row << row+count;

				q.bindValue(":id",  parentItem->children[i]->id );
				q.bindValue(":did", parentItem->id);

				if ( !q.exec() )
					qDebug() << q.executedQuery() << db.lastError().text();

					/*  For complete dezztruction of element :)
						q.clear();

						q.prepare(QString("DELETE FROM el_%1 WHERE id=%2").arg(hashTypes[parentItem->children[i]->id])
						.arg(parentItem->children[i]->id));
						q.exec(); */
			}

			for ( int i = row; i < row+count; i++ ) {
				hashTreeItems[parentItem->children.at(row)->id].removeAll(parentItem->children.at(row));

				cleanupTree(parentItem->children.at(row));
				delete parentItem->children.at(row);

				parentItem->children.removeAt(row);
				hashChildren[parentItem->id].removeAt(row);
			}

			for ( int i = 0; i < parentItem->children.size(); i++ ) {
				parentItem->children[i]->row = i;
			}

			hashChildCount[parentItem->id] -= count;

		endRemoveRows();
	}

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
	Q_UNUSED(row);

	if (action == Qt::IgnoreAction)
		return true;
	if (!data->hasFormat("application/x-real-uml-data"))
		return false;
	if (column > 0)
		return false;

	RepoTreeItem *parentItem;
	if ( parent.isValid() ) 
		parentItem = static_cast<RepoTreeItem *>(parent.internalPointer());
	else
		parentItem = rootItem;

	QByteArray dragData = data->data("application/x-real-uml-data");
	QDataStream stream(&dragData, QIODevice::ReadOnly);

	QString name;
	int newid, newtype;
	QPointF newPos;

	stream >> newid;
	stream >> newtype;
	stream >> name;
	stream >> newPos;

	qDebug() << "dropMimeData" << parentItem->id << newtype << newid;

	QSqlQuery q(db);

	switch (type(parentItem)) {
		case Category:
			{
				if ( parentItem->id != newtype ) {
					qDebug() << "Object dragged into the wrong category";
					return false;
				}

				q.prepare("INSERT INTO nametable (id, type, name) VALUES (null, :type, :name) ;");
				if ( newid == -1 ) {
					q.bindValue(":type", newtype);
					q.bindValue(":name", "(anon element)");

					beginInsertRows(parent, hashChildCount[parentItem->id], hashChildCount[parentItem->id]);

					if ( q.exec() ) {
						newid = q.lastInsertId().toInt();
						q.clear();

						// FIXME: Fix numbering... don't know how to do better.
						if ( newid < 1000 ) {
							q.prepare("UPDATE nametable SET id=id+1000 WHERE id=:id ;");
							q.bindValue(":id", newid);
							if ( q.exec() )
								newid += 1000;
							q.clear();
						}

						q.prepare(QString("INSERT INTO el_%1 (id) VALUES (:id) ;").arg(newtype));
						q.bindValue(":id", newid);
						q.exec();

						createItem(parentItem, newid, newtype);
					} else 
						qDebug() << q.executedQuery() << db.lastError().text();

					endInsertRows();
				}

				return true;
			}
			break;
		case Container:
			{
				int diagram_id = parentItem->id;

				if ( action == Qt::CopyAction ) {

					q.prepare("INSERT INTO nametable (id, type, name) VALUES (null, :type, :name) ;");
					if ( newid == -1 ) {
						q.bindValue(":type", newtype);
						q.bindValue(":name", "(anon element)");

						beginInsertRows(index(newtype-1,0,QModelIndex()),
								hashChildCount[newtype], hashChildCount[newtype]);

						if ( q.exec() ) {
							newid = q.lastInsertId().toInt();
							q.clear();

							q.prepare(QString("INSERT INTO el_%1 (id) VALUES (:id) ;").arg(newtype));
							q.bindValue(":id", newid);
							q.exec();

							createItem(rootItem->children.at(newtype-1), newid, newtype);
						} else 
							qDebug() << q.executedQuery() << db.lastError().text();

						endInsertRows();
					}

					q.prepare("INSERT INTO diagram (diagram_id, el_id, x, y) VALUES (:did, :elid, :x, :y) ;");
					q.bindValue(":did",diagram_id);
					q.bindValue(":elid",newid);
					q.bindValue(":x", newPos.x() );
					q.bindValue(":y", newPos.y() );

					if ( !q.exec() )
						qDebug() << q.executedQuery() << db.lastError().text();
				}

				beginInsertRows(parent, hashChildCount[parentItem->id], hashChildCount[parentItem->id]);
				createItem(parentItem, newid, newtype);
				hashDiagramElements[parentItem->id][newid].position = newPos.toPoint();
				endInsertRows();

				// updateRootTable();
			}
			break;
		default:
			return false;
	}

	return false;
}

void RealRepoModel::beginTransaction()
{
	if ( ! db.transaction() )
		qDebug() << db.lastError().text();
}

void RealRepoModel::commitTransaction()
{
	if ( ! db.commit() )
		qDebug() << db.lastError().text();
}

void RealRepoModel::rollbackTransaction()
{
	if ( db.rollback() ) {
		// FIXME: find more elegant way
		hashNames.clear();
		hashTypes.clear();
		hashChildren.clear();
		hashTreeItems.clear();
		hashChildCount.clear();
		hashElementProps.clear();
		hashDiagramElements.clear();

		cleanupTree(rootItem);
		readRootTable();

		reset();
	} else
		qDebug() << db.lastError().text();
}

RealRepoModel::ElementType RealRepoModel::type(const RepoTreeItem *item) const
{
	if ( item->id >= 100 )
		return Container;
	else if ( item->id != 0 )
		return Category;
	else
		return Root;
}

RealRepoModel::ElementType RealRepoModel::type(const QModelIndex &index) const
{
	if (index.isValid())
		return type(static_cast<RepoTreeItem *>(index.internalPointer()));
	else
		return Root;
}

void RealRepoModel::cleanupTree(RepoTreeItem *root)
{
	foreach (RepoTreeItem *childItem, root->children) {
		cleanupTree(childItem);
		delete childItem;
	}
	root->children.clear();
}

void RealRepoModel::createItem(RepoTreeItem *parentItem, int id, int type)
{
	RepoTreeItem *item = new RepoTreeItem;
	item->parent = parentItem;
	item->id = id;
	item->row = parentItem->children.size();

	parentItem->children.append(item);
	hashTreeItems[id].append(item);

	hashNames[id] = "(anon element)";
	hashTypes[id] = type;
	hashChildCount[id] = 0;

	hashChildren[parentItem->id].append(id);
	hashChildCount[parentItem->id]++;
}

void RealRepoModel::updateProperties(int id)
{
	QSqlQuery q(db);
	int type = hashTypes[id];

	QString sql = QString("SELECT `id`, `%1` FROM el_%2 WHERE id = :id ;")
		.arg(info.getColumnNames(type).join("`, `")).arg(type);

	q.prepare(sql);
	q.bindValue(":id", id);

	QMap <int, QVariant> properties;

	if ( q.exec() ) {
		if ( q.next() )
			for ( int i = 1; i < q.record().count(); i++ )
				properties[info.roleByIndex(i-1)] = q.value(i);
		else
			qDebug() << "some weird error";
	} else
		qDebug() << q.executedQuery() << db.lastError().text();

	hashElementProps[id] = properties;
}

void RealRepoModel::updateRootTable()
{
	// FIXME: call signals!!!! or rewrite the other way
	QSqlQuery q(db);
	if ( q.exec( "SELECT metatable.id, metatable.name, metatable.qualifiedName, COUNT(nametable.id)"
				"  FROM metatable LEFT JOIN nametable ON nametable.type=metatable.id"
				"  GROUP BY metatable.id;" ) ) {
		while ( q.next() ) {
			hashChildCount[q.value(0).toInt()] = q.value(3).toInt();
		}
	} else
		qDebug() << q.executedQuery() << db.lastError().text();
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
		qDebug() << q.executedQuery() << db.lastError().text();
}

void RealRepoModel::readCategoryTable(RepoTreeItem *parent)
{
	QSqlQuery q(db);
	q.prepare("SELECT nametable.id, nametable.name, nametable.type, nametable.qualifiedName,"
			" metatable.name, COUNT(diagram.diagram_id) FROM nametable"
			"  LEFT JOIN metatable ON metatable.id = nametable.type"
			"  LEFT JOIN diagram ON diagram.diagram_id = nametable.id"
			"  WHERE nametable.type = :type"
			"  GROUP BY nametable.id"
			"  ORDER BY diagram.diagram_id DESC ;");

	q.bindValue(0, parent->id);

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
			item->parent = parent;
			item->id = q.value(0).toInt();
			item->row = i++;
			parent->children.append(item);
			hashTreeItems[item->id].append(item);

			// Some elements may be already added here - user may open diagrams before us.
			if ( ! hashNames.contains(item->id) ) {
				hashNames[item->id] = q.value(1).toString();
				hashTypes[item->id] = q.value(2).toInt();
				hashChildCount[item->id] = q.value(5).toInt();
				hashChildren[parent->id].append(item->id);
			}
		}
	} else
		qDebug() << q.executedQuery() << db.lastError().text();
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
		q.prepare("SELECT diagram.el_id, nametable.name, nametable.type, diagram.x, diagram.y, diagram.cfg,"
				" nametable.qualifiedName, COUNT(children.el_id) FROM diagram"
				"  LEFT JOIN nametable ON diagram.el_id = nametable.id"
				"  LEFT JOIN diagram AS children ON children.diagram_id = diagram.el_id"
				"  WHERE diagram.diagram_id = :type"
				"  GROUP BY diagram.el_id ;");

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

				hashChildCount[item->id] = q.value(7).toInt();
				hashChildren[root->id].append(item->id);

				root->children.append(item);
				hashTreeItems[item->id].append(item);

				hashDiagramElements[root->id][item->id].position = QPoint(q.value(3).toInt(), q.value(4).toInt());

				// FIXME: parse some better way
				QPolygon newConfig; 
				QStringList pointList = q.value(5).toString().split(';',QString::SkipEmptyParts);
				foreach ( QString pointData, pointList ) {
					QStringList coords = pointData.split(QRegExp("\\(|\\)|,"),QString::SkipEmptyParts);
					newConfig << QPoint(coords.at(0).toInt(), coords.at(1).toInt());
				}

				hashDiagramElements[root->id][item->id].configuration = newConfig;
			}
		} else
			qDebug() << q.executedQuery() << db.lastError().text();
	}
}
