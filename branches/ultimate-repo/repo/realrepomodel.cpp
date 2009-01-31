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

RealRepoModel::RealRepoModel(QObject *parent)
	: QAbstractItemModel(parent)
{
dbg;
	m_error = -1;
	repoClient = new RealRepoClient();

	rootItem = new RepoTreeItem;
	rootItem->parent = 0;
	rootItem->id = 0;

	readRootTable();

	// Uncomment this when needed. Make sure that splashscreen turned off :)
	// runTestQueries();

	undoStack = new QUndoStack();
	undoView = new QUndoView(undoStack);
	undoView->setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint);
	undoView->setWindowTitle(tr("Command List"));
	undoView->setAttribute(Qt::WA_QuitOnClose, false);

	addToStack = true;
}

RealRepoModel::~RealRepoModel()
{
dbg;
	cleanupTree(rootItem);
	delete rootItem;
	undoView->close();
	delete undoView;
	delete undoStack;
}

QModelIndex RealRepoModel::createDefaultTopLevelItem() {
	RepoTreeItem *diagramCategory = hashTreeItems[UML::krnnDiagram].first();
	readCategoryTable(diagramCategory);
	if (diagramCategory->children.empty()) {
		addElementToModel(diagramCategory, index(diagramCategory), -1,
			UML::krnnDiagram, "Root diagram", QPointF(), Qt::CopyAction);
	}

	if (!diagramCategory->children.empty()) {
		return index(diagramCategory->children[0]);
	}
	else
		return QModelIndex();
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
//		if( curItem->row != 0){
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
	RepoTreeItem *parentItem;

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

	return hashChildCount[parentItem->id];
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

	if ( 1 ){ //type( parentItem ) == Container ) {
		beginRemoveRows(parent,row,row+count-1);
		for ( int i = row; i < row+count; i++ ){
//			qDebug() << "deleting element " << parentItem->children[i]->id;
			repoClient->deleteObject(parentItem->children[i]->id, parentItem->id);
		}

		for ( int i = row; i < row+count; i++ ) {
			hashTreeItems[parentItem->children.at(row)->id].removeAll(parentItem->children.at(row));

			cleanupTree(parentItem->children.at(row));
			delete parentItem->children.at(row);

			parentItem->children.removeAt(row);
			hashChildren[parentItem->id].removeAt(row);
		}

		for ( int i = 0; i < parentItem->children.size(); i++ )
			parentItem->children[i]->row = i;

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
	stream << hashNames[item->id];
	stream << hashDiagramElements[item->parent->id][item->id].position;

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
	int newid = 0, newtype = 0;
	QPointF newPos;

	stream >> newid;
	stream >> newtype;
	stream >> name;
	stream >> newPos;

	qDebug() << "dropped" << newid << newtype << name << newPos;

//	qDebug() << "dropMimeData" << parentItem->id << newtype << newid;
	return addElementToModel(parentItem, parent, newid, newtype, name, newPos,
		action);
}

bool RealRepoModel::addElementToModel(RepoTreeItem *const parentItem,
	const QModelIndex &parent, int const newid, int const newtype,
	QString const &name, QPointF const &newPos, Qt::DropAction action)
{
	switch (type(parentItem)) {
		case Category:
			{
//				qDebug() << parentItem->id << newtype;

				// Очередной быстрый фикс, к багу #96. На сей раз проблема в том,
				// что корневым элементом (благодаря колдовству с ID-шниками,
				// творящемуся ниже) у нас может быть только элемент типа krnnDiagram,
				// поэтому пришлось захардкодить особое разрешение для cnClassView.
				// Другое возможное решение - добавить в krnnDiagram какое-нибудь
				// свойство, по которому можно отличить диаграмму от представления.
				// Однако, нам хочется, чтобы диаграммы и представления отличались
				// визуально.
				// TODO: Такое чувство, что Category тут неспроста. Переделать, чтобы было красиво и правильно.
				if ( parentItem->id != newtype && newtype != UML::cnClassView) {
					qDebug() << "Object dragged into the wrong category";
					return false;
				}

				beginInsertRows(parent, hashChildCount[parentItem->id], hashChildCount[parentItem->id]);
				// FIXME
				int id = repoClient->createObject(newtype, "anonymous");
//				qDebug() << "\tcreating new item3" << parentItem->id << id << newtype;
				createItem(parentItem, id, newtype);
				endInsertRows();

				return true;
			}
			break;
		case Container:
			{
				qDebug() << "adding to container, action is " << action;
				int id = -1;

				// drag'n'drop из эксплорера, создаем ссылку на текущий элемент
				bool newElement = ( name != "(anon element)" );

				// drag'n'drop из палитры, создаем новый элемент
				if ( action == Qt::CopyAction ) { // дерево инстпектора об'ектов
					qDebug() << "Qt::CopyAction";
					beginInsertRows(index(newtype-1,0,QModelIndex()),
								hashChildCount[newtype], hashChildCount[newtype]);
					//FIXME
					if( newElement )
						id = repoClient->copyEntity(newtype, newid, parentItem->id);
					else
						id = repoClient->createObjectWithParent(newtype,"anonymous", parentItem->id);
					repoClient->setPosition(id, parentItem->id, (int) newPos.x(), (int) newPos.y());
					qDebug() << "\tcreating new item" << rootItem->children.at(newtype-1)->id << id << newtype;
					createItem(rootItem->children.at(newtype-1), id, newtype);
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

				// дерево инспектора диаграмм
				beginInsertRows(parent, hashChildCount[parentItem->id], hashChildCount[parentItem->id]);
				if( newElement )
					createItem(parentItem, id, newtype, name);
				else {
					// Если объект подвесили и в корень, и как сына другого объекта,
					// надо дать об этом знать репозиторию, иначе будет #95.
					// TODO: Subject to refactoring.
					if (action == Qt::CopyAction ) {
						id = repoClient->copyEntity(newtype, id, parentItem->id);
					}
					createItem(parentItem, id, newtype);
				}
				hashDiagramElements[parentItem->id][id].position = newPos.toPoint();
				endInsertRows();
			}
			break;
		default:
			return false;
	}

	return false;
}

RealRepoModel::ElementType RealRepoModel::type(const RepoTreeItem *item) const
{
dbg;
	if ( item->id >= 200 )
		return Container;
	else if ( item->id != 0 )
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

void RealRepoModel::createItem(RepoTreeItem *parentItem, int id, int type)
{
dbg;
	RepoTreeItem *item = new RepoTreeItem;
	item->parent = parentItem;
	item->id = id;
	item->row = parentItem->children.size();

// 	qDebug() << "++ id: " << id << ", children: " << item->row+1;
	parentItem->children.append(item);
	hashTreeItems[id].append(item);

	hashNames[id] = "anonymous";
	hashTypes[id] = type;
	hashChildCount[id] = 0;

	hashChildren[parentItem->id].append(id);
	hashChildCount[parentItem->id]++;
}

void RealRepoModel::createItem(RepoTreeItem *parentItem, int id, int type, QString name){
dbg;
	RepoTreeItem *item = new RepoTreeItem;
	item->parent = parentItem;
	item->id = id;
	item->row = parentItem->children.size();

	parentItem->children.append(item);
	hashTreeItems[id].append(item);

	hashNames[id] = name;
	hashTypes[id] = type;
	// FIXME
	hashChildCount[id] = 0;

	hashChildren[parentItem->id].append(id);
	hashChildCount[parentItem->id]++;
}

void RealRepoModel::updateProperties(int /*id*/)
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

void RealRepoModel::readRootTable()
{
dbg;

	int types = repoClient->getTypesCount();
	if( types == 0 ){
		m_error = repoClient->getLastError();
		qDebug() << "MODEL: error " << m_error;
		return;
	}
	for( int i=1; i<=types; i++ ){
		RealType info = repoClient->getTypeById(i);
		RepoTreeItem *item = new RepoTreeItem;
		int count = info.getObjects().size();
	//	qDebug() << "root table: " << info.getId() << count << info.getName() << info.getDescription() << item;
		item->parent = rootItem;
		item->row = i-1;
		item->id = info.getId();

		hashNames[item->id] = info.getName();
		hashChildCount[item->id] = count;
		hashTreeItems[item->id].append(item);

		rootItem->children.append(item);
	}

	hashChildCount[rootItem->id] = rootItem->children.size();
//	qDebug() << "root children" << rootItem->children.size();
}

void RealRepoModel::readCategoryTable(RepoTreeItem * parent)
{
dbg;
	// Select all elements of the same type as the parent

	QStringList ids = repoClient->getObjectsByType( parent->id ).split("\t", QString::SkipEmptyParts);

//	qDebug() << "searching for type " << parent->id << ", found " << ids.size() << "elements" << ids;
	for( int i=0; i<ids.size(); i++){
		QString data = repoClient->getObjectData(ids[i].toInt());
//		qDebug() << "element" << i << data;
		RepoTreeItem *item = new RepoTreeItem;
		item->parent = parent;
		item->id = ids[i].toInt();
		item->row = i;
		parent->children.append(item);
		hashTreeItems[item->id].append(item);

		// Some elements may be already added here - user may open diagrams before us.
		if ( ! hashNames.contains(item->id) ) {
			hashNames[item->id] = data.section("\t",1,1);
			hashTypes[item->id] = parent->id;
			hashChildCount[item->id] = data.section("\t",4,4).toInt();
			hashChildren[parent->id].append(item->id);
		}

	}
}

void RealRepoModel::readContainerTable(RepoTreeItem * root)
{
dbg;
//	qDebug() << "================ READING DIAGRAM =======================";
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
		QStringList children = repoClient->getChildren(root->id).split("\t", QString::SkipEmptyParts);
//		qDebug() << children.size() << children;
		for( int i=0; i<children.size(); i++ ){
			int _id = children[i].toInt();
			QString data = repoClient->getObjectData(_id);
//			int _type = data.section("\t",2,2).toInt();
			QString coordinates = repoClient->getPosition(_id, root->id);

			RepoTreeItem *item = new RepoTreeItem;
			item->parent = root;
			item->row = i;
			item->id = _id;

			hashNames[item->id] = data.section("\t",1,1);
			hashTypes[item->id] = data.section("\t",2,2).toInt();

			hashChildCount[item->id] = data.section("\t",4,4).toInt();
					hashChildren[root->id].append(item->id);

			root->children.append(item);
			hashTreeItems[item->id].append(item);

			hashDiagramElements[root->id][item->id].position =
				QPoint(coordinates.section(";",0,0).toInt(), coordinates.section(";",1,1).toInt());

			// FIXME: parse some better way
			QPolygon newConfig;
			QStringList pointList = repoClient->getConfiguration(_id, root->id).split(';',QString::SkipEmptyParts);
			foreach ( QString pointData, pointList ) {
				QStringList coords = pointData.split(QRegExp("\\(|\\)|,"),QString::SkipEmptyParts);
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

void RealRepoModel::runTestQueries()
{

	qDebug() << "Getting types by metatype 'object'";
	QIntList result = repoClient->getTypesByMetaType(object);
	qDebug() << "Result: " << result;

	qDebug() << "Getting types by metatype 'link'";
	result = repoClient->getTypesByMetaType(QRealTypes::link);
	qDebug() << "Result: " << result;

	qDebug() << "Getting types by metatype 'dataType'";
	result = repoClient->getTypesByMetaType(dataType);
	qDebug() << "Result: " << result;

	qDebug() << "Getting types by metatype 'rawType'";
	result = repoClient->getTypesByMetaType(rawType);
	qDebug() << "Result: " << result;

	qDebug() << "Getting type ID by name for non-existing (for now!) type 'Cthulhu fhtagn!'";
	int intResult = repoClient->getTypeIdByName("Cthulhu fhtagn!");
	qDebug() << "Result: " << intResult;

	qDebug() << "Done";
}
