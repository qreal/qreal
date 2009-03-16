/** @file realrepomodel.cpp
 *	@brief Основная модель данных
 * */
#include <QtGui>
#include <QtSql>
#include <QMessageBox>

#include "realrepomodel.h"
#include "realreporoles.h"
//#define _LONG_DEBUG
#include "dbg.h"

using namespace qRealTypes;

RealRepoModel::RealRepoModel( const QString &addr, const int port, QObject *parent )
	: QAbstractItemModel(parent)
{
dbg;
	m_error = -1;
	qDebug() << addr << port;
	repoClient = new RealRepoClient(addr, port, this);

	rootItem = new RepoTreeItem;
	rootItem->parent = NULL;
	rootItem->id = "root";

	if (!readRootTable())
		exit(1);

	// Uncomment this when needed. Make sure that splashscreen turned off :)
	// runTestQueries();

	undoStack = new QUndoStack();
	undoView = new QUndoView(undoStack);
	undoView->setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint);
	undoView->setWindowTitle(tr("Command List"));
	undoView->setAttribute(Qt::WA_QuitOnClose, false);

	addToStack = true;

	readItems();
}

RealRepoModel::~RealRepoModel()
{
dbg;
	cleanupTree(rootItem);
	delete rootItem;
	undoView->close();
	delete undoView;
	delete undoStack;
	delete repoClient;
}

QModelIndex RealRepoModel::createDefaultTopLevelItem() {
	RepoTreeItem *diagramCategory = hashTreeItems["krnnDiagram"].first();
	if (diagramCategory->children.empty()) {
		addElementToModel(diagramCategory, index(diagramCategory), "", "",
			"krnnDiagram", "Root diagram", QPointF(), Qt::CopyAction, -1);
	}

	if (!diagramCategory->children.empty()) {
		return index(diagramCategory->children[0]);
	}
	else
		return QModelIndex();
}

void RealRepoModel::readItems()
{
	RepoTreeItem *diagramCategory = hashTreeItems["krnnDiagram"].first();
	readCategoryTable(diagramCategory);
	foreach (RepoTreeItem *diagram, diagramCategory->children) {
		readItemsRecurse(diagram);
	}
}

void RealRepoModel::readItemsRecurse( RepoTreeItem *parent )
{
	readContainerTable(parent);
	foreach (RepoTreeItem *childItem,parent->children) {
		readItemsRecurse(childItem);
	}
}

QVariant RealRepoModel::data(const QModelIndex &index, int role) const
{
dbg;
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
//					return info.objectIcon(item->id);
					return QIcon();
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
				RealRepoInfo info;
				QString name = info.getColumnName(hashTypes[item->id], role);
//				qDebug() << "requested role:" << role << name;
				QString val = repoClient->getPropValue(item->id, name);
				return (val == "") ? QVariant() : val;
/*
				if ( hashElementProps.contains(item->id) ) {
					if ( hashElementProps[item->id].contains(role) ){
						return hashElementProps[item->id][role];
					}
					else
						return QVariant();
				} else {
					const_cast<RealRepoModel *>(this)->updateProperties(item->id);
					qDebug() << "returning" << hashElementProps[item->id][role];
					return hashElementProps[item->id][role];
				}	*/
			} else {
				return QVariant(); //for now
			}
	}

	return QVariant();
}

bool RealRepoModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
dbg;
//	qDebug() << __PRETTY_FUNCTION__;
	if (!index.isValid())
		return false;

	if (index.column() != 0)
		return false;

	RepoTreeItem *item = static_cast<RepoTreeItem*>(index.internalPointer());
//	qDebug() << "role:" << role;
	switch (role) {
		case Qt::DisplayRole:
		case Unreal::krnnNamedElement::nameRole:
		case Qt::EditRole:
			{
				repoClient->setName(item->id, value.toString());
				hashNames[item->id] = value.toString();
			}
			break;
		case Unreal::PositionRole:
			{
				if ( type(item->parent) == Container ) {
					if( addToStack ){
						undoStack->push(new ChangePositionCommand(this, index,
									QVariant(hashDiagramElements[item->parent->id][item->id].position), value, role));
					}
					repoClient->setPosition(item->id, item->parent->id, value.toPoint().x(), value.toPoint().y());
					hashDiagramElements[item->parent->id][item->id].position = value.toPoint();
				}
				break;
			}
		case Unreal::ConfigurationRole:
			{
				if ( type(item->parent) == Container ) {

					QPolygon poly(value.value<QPolygon>());
					QString result;
					foreach ( QPoint point, poly ) {
						result += QString("(%1,%2);").arg(point.x()).arg(point.y());
					}
					result.chop(1);

					if( addToStack ){
						undoStack->push(new ChangeConfigurationCommand(this, index,
									QVariant(hashDiagramElements[item->parent->id][item->id].configuration), value, role));
					}
					repoClient->setConfiguration(item->id, item->parent->id, result);
					hashDiagramElements[item->parent->id][item->id].configuration = poly;
				}
				break;
			}
		default:
//			qDebug() << "role -- " << role;
			if ( role >= Unreal::UserRole ) {

				// FIXME
				repoClient->setPropValue(item->id,
					info.getColumnName(hashTypes[item->id],role), value.toString());

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
dbg;
	switch ( type(index) ) {
		case Container:		return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable
							 | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
		case Category:		return Qt::ItemIsEnabled | Qt::ItemIsDropEnabled;
		case Root:			return Qt::ItemIsEnabled;
		default:			return 0;
	}
}

QVariant RealRepoModel::headerData(int section, Qt::Orientation orientation,
		int role) const
{
dbg;
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 0 )
		return tr("Name");

	return QVariant();
}

QModelIndex RealRepoModel::index(const RepoTreeItem *item) const
{
dbg;
	QList <int> rowCoords;

	for ( RepoTreeItem *curItem = const_cast<RepoTreeItem *>(item);
			curItem != rootItem; curItem = curItem->parent ) {
	// WT* is this if?
//		if( curItem && curItem->row != 0){
			rowCoords.append(curItem->row);
//		}
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
dbg;
	RepoTreeItem *parentItem = NULL;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<RepoTreeItem*>(parent.internalPointer());

//	qDebug() << "[INDEX]: id: " << parentItem->id << "row:" << row << "column:" << column << "children:" << parentItem->children.size();

	if ( parentItem->children.isEmpty() ) {
		if (type(parentItem) == Container ) {
			const_cast<RealRepoModel *>(this)->readContainerTable(parentItem);
		} else {
			const_cast<RealRepoModel *>(this)->readCategoryTable(parentItem);
		}
	}

	RepoTreeItem *childItem = 0;
	if( parentItem && parentItem->children.size() > row && row >= 0 )
		childItem = parentItem->children[row];

//	qDebug() << parentItem->children.size() << row << parentItem->id;
//	RepoTreeItem *childItem = parentItem->children[row];

	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}

QModelIndex RealRepoModel::parent(const QModelIndex &child) const
{
dbg;
	if (!child.isValid())
		return QModelIndex();

	RepoTreeItem *childItem = static_cast<RepoTreeItem*>(child.internalPointer());
	RepoTreeItem *parentItem = 0;
	if( childItem )
		parentItem = childItem->parent;

	if (parentItem == rootItem)
		return QModelIndex();

	return createIndex(parentItem->row, 0, parentItem);
}

int RealRepoModel::rowCount(const QModelIndex &parent) const
{
dbg;
	RepoTreeItem *parentItem;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<RepoTreeItem*>(parent.internalPointer());

	return parentItem->children.size();
}

int RealRepoModel::columnCount(const QModelIndex &/*parent*/) const
{
dbg;
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

	for ( int i = row; i < row+count; i++ ){
		//                      qDebug() << "deleting element " << parentItem->children[i]->id;
		repoClient->deleteObject(parentItem->children[i]->id, parentItem->id);
	}

	removeChildrenRows(parent,parentItem,row,count);

	return true;
}

void RealRepoModel::removeChildrenRows( QPersistentModelIndex parent, RepoTreeItem* parentItem, int row, int count )
{
	beginRemoveRows(parent,row,row+count-1);

	for ( int i = row; i < row+count; i++ ) {
		hashTreeItems[parentItem->children.at(row)->id].removeAll(parentItem->children.at(row));

		cleanupTree(parentItem->children.at(row));
		delete parentItem->children.at(row);

		parentItem->children.removeAt(row);
	}

	for ( int i = 0; i < parentItem->children.size(); i++ )
		parentItem->children[i]->row = i;

	endRemoveRows();
}

QStringList RealRepoModel::mimeTypes () const
{
	QStringList types;
	types << "application/x-real-uml-data";
	return types;
}

QMimeData * RealRepoModel::mimeData ( const QModelIndexList & indexes ) const
{
dbg;
	qDebug() << "++++++++++++++++++!!!!!!!!!!!!!!!!++++++++++++++++++";
	qDebug() << "index list size: " << indexes.size();

	RepoTreeItem *item;
	if ( indexes.at(0).isValid() )
		item = static_cast<RepoTreeItem *>(indexes.at(0).internalPointer());
	else{
		qDebug() << "bad item dragged!";
		return 0;
	}

	QByteArray itemData;
	QDataStream stream(&itemData, QIODevice::WriteOnly);
	stream << item->id;
	stream << hashTypes[item->id];
	stream << item->parent->id;
	stream << hashNames[item->id];
	stream << hashDiagramElements[item->parent->id][item->id].position;
	stream << item->row;

	qDebug() << "ID: 	 " << item->id;
	qDebug() << "TYPE: 	 " << hashTypes[item->id];
	qDebug() << "PARENT: " << item->parent->id;

	QMimeData *mimeData = new QMimeData;
	mimeData->setData("application/x-real-uml-data", itemData);
	return mimeData;
}

Qt::DropActions RealRepoModel::supportedDropActions () const
{
dbg;
	return Qt::CopyAction | Qt::MoveAction | Qt::LinkAction;
}

bool RealRepoModel::dropMimeData(const QMimeData *data, Qt::DropAction action,
		int row, int column, const QModelIndex &parent)
{
dbg;
	Q_UNUSED(row);

	if (action == Qt::IgnoreAction)
		return true;
	if (!data->hasFormat("application/x-real-uml-data"))
		return false;
	if (column > 0)
		return false;

//	qDebug() << "parent is valid" << parent.isValid();
	RepoTreeItem *parentItem = rootItem;
	if (parent.isValid())
		parentItem = static_cast<RepoTreeItem *>(parent.internalPointer());

	QByteArray dragData = data->data("application/x-real-uml-data");
	QDataStream stream(&dragData, QIODevice::ReadOnly);

	QString name;
	IdType newtype = "", oldParent = "";
	TypeIdType newid = "";
	QPointF newPos;
	int oldRow;

	stream >> newid;
	stream >> newtype;
	stream >> oldParent;
	stream >> name;
	stream >> newPos;
	stream >> oldRow;

//	qDebug() << "dropped" << newid << newtype << name << newPos;

	qDebug() << "dropMimeData" << parentItem->id << newtype << newid;
	return addElementToModel(parentItem, parent, oldParent, newid, newtype, name, newPos,
		action,oldRow);
}

bool RealRepoModel::addElementToModel( RepoTreeItem *const parentItem, const QModelIndex &parent,
									  IdType const &oldParent, IdType const &newid, 
									  TypeIdType const &newtype, QString const &name,
									  QPointF const &newPos, Qt::DropAction action, int oldRow )
{
	switch (type(parentItem)) {
		case Category:
			{
//				qDebug() << parentItem->id << newtype;
				qDebug() << newid << newtype << name << newPos;

				if ( parentItem->id != newtype) {
					qDebug() << "Object dragged into the wrong category";
					return false;
				}

				beginInsertRows(parent, parentItem->children.size(), parentItem->children.size());
				// FIXME
				IdType id = repoClient->createObject(newtype, "anonymous");
//				qDebug() << "\tcreating new item3" << parentItem->id << id << newtype;
				createItem(parentItem, id, newtype);
				endInsertRows();

				return true;
			}
			break;
		case Container:
			{
				qDebug() << "adding to container, action is " << action;
				IdType id = newid;
				qDebug() << "newid: " << newid;

				// drag'n'drop из эксплорера, создаем ссылку на текущий элемент
				bool newElement = (id == "");
				qDebug() << newElement << name;
				CopyType copyType = SYM_LINK_TYPE;

				if (!newElement) {

					QMenu menu;

					QAction *copyAction = menu.addAction("Perform full copy");
					QAction *symlinkAction = menu.addAction("Add symlink");

					if ( QAction *selectedAction = menu.exec(QCursor::pos()) ) {
						// hack with coords (model knows nothing about the GUI event)
						if (selectedAction == copyAction) {
							copyType = FULL_COPY_TYPE;
						} else if (selectedAction == symlinkAction) {
							copyType = SYM_LINK_TYPE;
						} else
							return false;
					} else
						return false;
				}

				// drag'n'drop из палитры, создаем новый элемент
				if (action == Qt::CopyAction && newElement) { // дерево инспектора объектов
					qDebug() << "Qt::CopyAction";

					int typeIndex = findIndex(newtype);
					QModelIndex newTypeIndex = index(typeIndex, 0, QModelIndex());
					RepoTreeItem *typeItem = static_cast<RepoTreeItem*>(newTypeIndex.internalPointer());
					beginInsertRows(newTypeIndex, typeItem->children.size(), typeItem->children.size());
					id = repoClient->createObjectWithParent(newtype,"anonymous", parentItem->id);
					repoClient->setPosition(id, parentItem->id, (int)newPos.x(), (int)newPos.y());
					qDebug() << "\tcreating new item" << rootItem->children.at(typeIndex)->id << id << newtype;
					createItem(rootItem->children.at(typeIndex), id, newtype);
					endInsertRows();
				}

				qDebug() << "\tcreating new item2" << parentItem->id << id << newtype;

				// TODO: What the hell?
				Q_ASSERT(action == Qt::CopyAction);
				if (parentItem->id == id) {
					QMessageBox::warning(NULL, tr("Cognitive hazard"),
						 tr("This is a diagram element, not a Klein bottle!"));
					return false;
				}
				foreach (RepoTreeItem *child, parentItem->children) {
					if (child->id == id && id != "") {
						QMessageBox::warning(NULL, tr("Warning!"),
							tr("Making two copies of one element with the same parent is not allowed, use containers instead."));
						return false;
					}
				}
				// дерево инспектора диаграмм
				if (newElement) {
					beginInsertRows(parent,  parentItem->children.size(), parentItem->children.size());
					createItem(parentItem, id, newtype, name);
					hashDiagramElements[parentItem->id][id].position = newPos.toPoint();
					endInsertRows();
				}
				else {
					// Если объект подвесили и в корень, и как сына другого объекта,
					// надо дать об этом знать репозиторию, иначе будет #95.
					// TODO: Subject to refactoring.
					if (action == Qt::CopyAction) {
						if (copyType == SYM_LINK_TYPE){
							qDebug() << "SYM_LINK_TYPE";

							Q_ASSERT(oldRow!=-1);
							RepoTreeItem *sourceItem;
							foreach (RepoTreeItem *mbOldParent,hashTreeItems[oldParent]) {
								foreach (RepoTreeItem *mbItem, mbOldParent->children) {
									if ((mbItem->id == id) && (mbItem->row==oldRow)) {
										sourceItem = mbItem;
									}
								}
							}
							Q_ASSERT(sourceItem);

							id = repoClient->copyEntity(newtype, id, parentItem->id, oldParent);
							RepoTreeItem *newItem = copyElement(sourceItem,parent,parentItem);
							copyChildren(sourceItem,index(newItem),newItem);
						}
						else if (copyType == FULL_COPY_TYPE) {
							qDebug() << "FULL_COPY_TYPE";
							beginInsertRows(parent,  parentItem->children.size(), parentItem->children.size());

							id = repoClient->copyEntity(newtype, id, parentItem->id, oldParent, true);

							RepoTreeItem *newItem = createItem(parentItem, id, newtype, name);
							if (hashDiagramElements[oldParent].contains(newid)) { 
								hashDiagramElements[parentItem->id][id]= 
									 hashDiagramElements[oldParent][newid]; 
                            }   
							endInsertRows();
							readChildren(newItem);
						}

					} else {
						beginInsertRows(parent,  parentItem->children.size(), parentItem->children.size());
						createItem(parentItem, id, newtype);
						endInsertRows();
					}
				}
//				foreach( RepoTreeItem *item, hashTreeItems[id])
//					emit dataChanged(index(item),index(item));
			}
			break;
		default:
			return false;
	}

	return false;
}

void RealRepoModel::readChildren( RepoTreeItem *item )
{
	readContainerTable(item);
	beginInsertRows(index(item),0,item->children.size());
	endInsertRows();
	foreach (RepoTreeItem *child, item->children) {
		readChildren(child);
	}
}

RealRepoModel::RepoTreeItem* RealRepoModel::copyElement( RepoTreeItem *item, QPersistentModelIndex newParent, RepoTreeItem *parentItem )
{
	IdType oldParent = item->parent->id;
	IdType id = item->id;

	beginInsertRows(newParent, parentItem->children.size(), parentItem->children.size());

	RepoTreeItem *newItem = createItem(parentItem, id, hashTypes[id], hashNames[id]);
	if (hashDiagramElements[oldParent].contains(id)) {
		hashDiagramElements[parentItem->id][id] = hashDiagramElements[oldParent][id];
	}
	endInsertRows();

	return newItem;
}


void RealRepoModel::copyChildren( RepoTreeItem *item, QPersistentModelIndex newElem, RepoTreeItem *newItem )
{
	foreach (RepoTreeItem *childItem, item->children) {
		IdTypeList newChildren;
		foreach(RepoTreeItem *newChild,newItem->children) {
			newChildren.append(newChild->id);
		}
		if (!newChildren.contains(childItem->id)) {
			RepoTreeItem *newChildItem = copyElement(childItem,newElem,newItem);
			copyChildren(childItem,index(newChildItem),newChildItem);
		}
	}
}


void RealRepoModel::removeChildren( QPersistentModelIndex elem,RepoTreeItem* item )
{
	foreach (RepoTreeItem *childItem, item->children) {
		removeChildren(index(childItem),childItem);
		if (childItem->children.size()) {
			removeChildrenRows(index(childItem),childItem,0,childItem->children.size());
		}
	}
	if (item->children.size()) {
		removeChildrenRows(elem,item,0,item->children.size());
	}
}

void RealRepoModel::changeParent(QPersistentModelIndex elem,QPersistentModelIndex newParent, QPointF newPos)
{
	if (newParent!=parent(elem)) {
		setData(elem, newPos, Unreal::PositionRole);

		RepoTreeItem *item = static_cast<RepoTreeItem*>(elem.internalPointer());
		IdType oldParent = item->parent->id;
		RepoTreeItem *parentItem = static_cast<RepoTreeItem*>(newParent.internalPointer());
		IdType id = item->id;

		id = repoClient->copyEntity(hashTypes[id], id, parentItem->id, oldParent);

		RepoTreeItem *newItem = copyElement(item,newParent,parentItem);

		copyChildren(item,index(newItem),newItem);

		removeChildren(elem,item);

		removeRows(item->row,1,parent(elem));
	}
}

RealRepoModel::ElementType RealRepoModel::type(const RepoTreeItem *item) const
{
dbg;
	// TODO: Грязный хак. Задача - уметь отличать тип от объекта. У типов строковые
	// ид-шники совпадают с именами типов, у объектов это просто номера.
	if (item->id.toInt() != 0)
		return Container;
	else
	if (item->id != "root")
		return Category;
	else
		return Root;
}

RealRepoModel::ElementType RealRepoModel::type(const QModelIndex &index) const
{
dbg;
	if (index.isValid())
		return type(static_cast<RepoTreeItem *>(index.internalPointer()));
	else
		return Root;
}

void RealRepoModel::cleanupTree(RepoTreeItem *root)
{
dbg;
	foreach (RepoTreeItem *childItem, root->children) {
		cleanupTree(childItem);
		delete childItem;
	}
	root->children.clear();
}

RealRepoModel::RepoTreeItem* RealRepoModel::commonCreateItem( RepoTreeItem *parentItem, IdType const &id, TypeIdType const &type )
{
	//	qDebug() << "\n\n====================";
	RepoTreeItem *item = new RepoTreeItem;
	item->parent = parentItem;
	item->id = id;
	item->row = parentItem->children.size();

	// 	qDebug() << "++ id: " << id << ", children: " << item->row+1;
	parentItem->children.append(item);
	hashTreeItems[id].append(item);

	hashTypes[id] = type;

	//	qDebug() << "parent: " << parentItem->id << ", id: " << id;

	return item;
}

void RealRepoModel::createItem(RepoTreeItem *parentItem, IdType const &id, TypeIdType const &type)
{
dbg;
	commonCreateItem(parentItem,id,type);
	hashNames[id] = "anonymous";
}

RealRepoModel::RepoTreeItem* RealRepoModel::createItem(RepoTreeItem *parentItem, IdType const &id, TypeIdType const &type, QString name){
dbg;
	RepoTreeItem *item = commonCreateItem(parentItem,id,type);
	hashNames[id] = name;
	return item;
}

void RealRepoModel::updateProperties(IdType const & /*id*/)
{
dbg;
}

void RealRepoModel::updateRootTable()
{
dbg;

/*
	// FIXME: call signals!!!! or rewrite the other way

	QList< QPair<int,int> > list = repoClient->getDiagramsChildCount();
	int i=0;

	while ( i<list.size()  ) {
		hashChildCount[list[i].first] = list[i].second;
		i++;
	}*/
}

bool RealRepoModel::readRootTable()
{
dbg;
	unsigned count = 0;
	qRealTypes::TypeIdTypeList types = repoClient->getAllTypes();
	foreach (TypeIdType type, types) {
		RealType info = repoClient->getTypeById(type);
		RepoTreeItem *item = new RepoTreeItem;
		// qDebug() << "root table: " << info.getId() << count << info.getName() << info.getDescription() << item;
		item->parent = rootItem;
		item->row = count;
		item->id = info.getId();

		hashNames[item->id] = info.getName();

		hashTreeItems[item->id].append(item);

		rootItem->children.append(item);
		++count;
	}

//	qDebug() << "root children" << rootItem->children.size();
	return true;
}

void RealRepoModel::readCategoryTable(RepoTreeItem * parent)
{
dbg;
	// Select all elements of the same type as the parent

	IdTypeList ids = repoClient->getObjectsListByType(parent->id);
	qDebug() << ids;

//	qDebug() << "searching for type " << parent->id << ", found " << ids.size() << "elements" << ids;
	unsigned count = 0;
	foreach (IdType id, ids) {
		// TODO: не должно по идее быть так.
		if (id == "")
			continue;
		QString data = repoClient->getObjectData(id);
//		qDebug() << "element" << i << data;
		RepoTreeItem *item = new RepoTreeItem;
		item->parent = parent;
		item->id = id;
		item->row = count;
		parent->children.append(item);
		hashTreeItems[item->id].append(item);

		// Some elements may be already added here - user may open diagrams before us.
		if (!hashNames.contains(item->id)) {
			hashNames[item->id] = data.section("\t", 1, 1);
			hashTypes[item->id] = parent->id;
		}
		++count;
	}
}

void RealRepoModel::readContainerTable(RepoTreeItem * root)
{
dbg;
//	qDebug() << "================ READING DIAGRAM =======================";
	IdTypeList idChildren;
	foreach (RepoTreeItem *idItem, hashTreeItems[root->id]) {
		foreach(RepoTreeItem *idChild,idItem->children) {
			if (!idChildren.contains(idChild->id)) {
				idChildren.append(idChild->id);
			}
		}
	}

	if (idChildren.size()) {
		int i = 0;
		foreach (IdType childId, idChildren) {
			RepoTreeItem *item = new RepoTreeItem;
			item->parent = root;
			item->row = i++;
			item->id = childId;

			root->children.append(item);
			hashTreeItems[item->id].append(item);
		}
	} else {
		QStringList children = repoClient->getChildren(root->id).split("\t", QString::SkipEmptyParts);
//		qDebug() << children.size() << children;
		for (int i = 0; i < children.size(); ++i) {
			IdType id = children[i];
			QString data = repoClient->getObjectData(id);
//			int _type = data.section("\t",2,2).toInt();
			QString coordinates = repoClient->getPosition(id, root->id);

			RepoTreeItem *item = new RepoTreeItem;
			item->parent = root;
			item->row = i;
			item->id = id;

			hashNames[item->id] = data.section("\t", 1, 1);
			hashTypes[item->id] = data.section("\t", 2, 2);

			root->children.append(item);
			hashTreeItems[item->id].append(item);

			hashDiagramElements[root->id][item->id].position =
				QPoint(coordinates.section(";", 0, 0).toInt(), coordinates.section(";", 1, 1).toInt());

			// FIXME: parse some better way
			QPolygon newConfig;
			QStringList pointList = repoClient->getConfiguration(id, root->id).split(';', QString::SkipEmptyParts);
			foreach (QString pointData, pointList) {
				QStringList coords = pointData.split(QRegExp("\\(|\\)|,"), QString::SkipEmptyParts);
				newConfig << QPoint(coords.at(0).toInt(), coords.at(1).toInt());
			}
			hashDiagramElements[root->id][item->id].configuration = newConfig;
		}

	}
}

void RealRepoModel::undo()
{
	qDebug() << "undo";
	undoStack->undo();
}

void RealRepoModel::redo()
{
	qDebug() << "redo";
	undoStack->redo();
}

void RealRepoModel::showCommandList()
{
	undoView->show();
}

void RealRepoModel::safeSetData(const QModelIndex & index, const QVariant & value, int role)
{
	addToStack = false;
	setData(index, value, role);
	addToStack = true;
	emit dataChanged(index,index);
}

unsigned RealRepoModel::findIndex(TypeIdType const &id) const
{
	unsigned count = 0;
	foreach (RepoTreeItem *item, rootItem->children) {
		if (item->id == id)
			return count;
		++count;
	}
	Q_ASSERT(!"Element not found in the root table");
	return count;
}

QModelIndex RealRepoModel::getDiagramCategoryIndex() const
{
	// Выглядит как хак, но сама по себе эта функция - хак и не должна пережить
	// введение плагинов.
	return index(findIndex("krnnDiagram"), 0, QModelIndex());
}

void RealRepoModel::runTestQueries()
{

	qDebug() << "Getting types by metatype 'object'";
	TypeIdTypeList result = repoClient->getTypesByMetaType(object);
	qDebug() << "Result: " << result;

	qDebug() << "Getting types by metatype 'link'";
	result = repoClient->getTypesByMetaType(qRealTypes::link);
	qDebug() << "Result: " << result;

	qDebug() << "Getting types by metatype 'dataType'";
	result = repoClient->getTypesByMetaType(dataType);
	qDebug() << "Result: " << result;

	qDebug() << "Getting types by metatype 'rawType'";
	result = repoClient->getTypesByMetaType(rawType);
	qDebug() << "Result: " << result;

	qDebug() << "Getting type ID by name for non-existing (for now!) type 'Cthulhu fhtagn!'";
	TypeIdType typeIdTypeResult = repoClient->getTypeIdByName("Cthulhu fhtagn!");
	qDebug() << "Result: " << typeIdTypeResult;

	qDebug() << "Done";
}