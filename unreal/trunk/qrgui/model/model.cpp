#include "model.h"
#include <QtCore/QDebug>
#include <QtGui/QPolygon>
#include <QtGui/QIcon>
#include <QUuid>

using namespace qReal;
using namespace model;

Model::Model(EditorManager const &editorManager)
		: mEditorManager(editorManager)
{
	rootItem = new ModelTreeItem(ROOT_ID, NULL);
	init();
	if (rootItem->children().count() == 0)
	{
		Id elementId(Id::loadFromString("qrm:/KernelEditor/Kernel/krnnDiagram"), QUuid::createUuid().toString());
		addElementToModel(rootItem, elementId, "", "(anonymous Diagram)", QPointF(0,0), Qt::CopyAction);
	}
}

Model::~Model()
{
	cleanupTree(rootItem);
	treeItems.clear();
}

void Model::init()
{
	treeItems.insert(ROOT_ID, rootItem);
	mApi.setName(ROOT_ID, ROOT_ID.toString());
	loadSubtreeFromClient(rootItem);
}

Qt::ItemFlags Model::flags(QModelIndex const &index) const
{
	if (index.isValid()) {
		return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled
			| Qt::ItemIsDropEnabled | Qt::ItemIsEnabled;
	} else {
		return Qt::ItemIsDropEnabled;  // У нас rootItem представляется невалидным индексом, а в него надо уметь кидать элементы.
	}
}

QVariant Model::data(QModelIndex const &index, int role) const
{
	if (index.isValid()) {
		ModelTreeItem *item = static_cast<ModelTreeItem*>(index.internalPointer());
		Q_ASSERT(item);
		switch (role) {
			case Qt::DisplayRole:
			case Qt::EditRole:
				return mApi.name(item->id());
			case Qt::DecorationRole:
				return mEditorManager.icon(item->id());
			case roles::idRole:
				return item->id().toVariant();
			case roles::positionRole:
				return mApi.property(item->id(), positionPropertyName(item));
			case roles::fromRole:
				return mApi.from(item->id()).toVariant();
			case roles::toRole:
				return mApi.to(item->id()).toVariant();
			case roles::fromPortRole:
				return mApi.fromPort(item->id());
			case roles::toPortRole:
				return mApi.toPort(item->id());
			case roles::configurationRole:
				return mApi.property(item->id(), configurationPropertyName(item));
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

bool Model::setData(QModelIndex const &index, QVariant const &value, int role)
{
	if (index.isValid()) {
		ModelTreeItem *item = static_cast<ModelTreeItem*>(index.internalPointer());
		switch (role) {
		case Qt::DisplayRole:
		case Qt::EditRole:
			mApi.setName(item->id(), value.toString());
			break;
		case roles::positionRole:
			mApi.setProperty(item->id(), positionPropertyName(item), value);
			break;
		case roles::configurationRole:
			mApi.setProperty(item->id(), configurationPropertyName(item), value);
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
		emit dataChanged(index, index);
		return true;
	}
	return false;
}

QString Model::findPropertyName(Id const &id, int const role) const
{
	// В случае свойства, описанного в самом элементе, роль - просто
	// порядковый номер свойства в списке свойств. Этого соглашения
	// надо всюду придерживаться, а то роли "поедут".
	QStringList properties = mEditorManager.getPropertyNames(id.type());
	Q_ASSERT(role - roles::customPropertiesBeginRole < properties.count());
	return properties[role - roles::customPropertiesBeginRole];
}

QVariant Model::headerData( int section, Qt::Orientation orientation, int role ) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 0 ) {
		return QVariant("name");
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

int Model::columnCount(QModelIndex const &parent) const
{
	Q_UNUSED(parent)
	return 1;
}

bool Model::removeRows(int row, int count, QModelIndex const &parent)
{
	ModelTreeItem *parentItem = rootItem;
	if (parent.isValid()) {
		parentItem = static_cast<ModelTreeItem*>(parent.internalPointer());
	}

	if (parentItem->children().size() < row + count) {
		return false;
	} else {
		for (int i = row; i < row + count; ++i) {
			removeModelItems(parentItem->children().at(i));

			// TODO: Убрать копипасту.
			ModelTreeItem *child = parentItem->children().at(i);
			int childRow = child->row();
			beginRemoveRows(parent, childRow, childRow);
			child->parent()->removeChild(child);
			treeItems.remove(child->id(), child);
			if (treeItems.count(child->id()) == 0) {
				mApi.removeChild(parentItem->id(), child->id());
			}
			delete child;
			endRemoveRows();
		}
		return true;
	}
}

QString Model::pathToItem(ModelTreeItem const *item) const
{
	if (item != rootItem) {
		QString path;
		do {
			item = item->parent();
			path = item->id().toString() + PATH_DIVIDER + path;
		} while (item!=rootItem);
		return path;
	}
	else
		return ROOT_ID.toString();
}

void Model::removeConfigurationInClient( ModelTreeItem *item )
{
	mApi.removeProperty(item->id(), positionPropertyName(item));
	mApi.removeProperty(item->id(), configurationPropertyName(item));
}

QModelIndex Model::index(ModelTreeItem *item)
{
	QList<int> rowCoords;

	for (ModelTreeItem const *curItem = item;
		curItem != rootItem; curItem = curItem->parent())
	{
		rowCoords.append(const_cast<ModelTreeItem *>(curItem)->row());
	}

	QModelIndex result;

	for (int i = rowCoords.size() - 1; i >= 0; --i) {
		result = index(rowCoords[i], 0, result);
	}

	return result;
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
			mApi.removeChild(root->id(),child->id());
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
			stream << mApi.property(item->id(), "name");
			stream << mApi.property(item->id(), positionPropertyName(item)).toPointF();
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
		QString pathToItem;
		QString name;
		QPointF position;
		stream >> idString;
		stream >> pathToItem;
		stream >> name;
		stream >> position;
		Id id = Id::loadFromString(idString);
		Q_ASSERT(id.idSize() == 4);  // Бросать в модель мы можем только конкретные элементы.

		if (treeItems.contains(id))  // Пока контейнеры не работают, бросать существующие элементы нельзя.
			return false;

		return addElementToModel(parentItem,id,pathToItem,name,position,action) != NULL;
	}
}

bool Model::isDiagram(const Id &id)
{
	return ((id.editor() == "KernelEditor") && (id.diagram() == "Kernel") && (id.element() == "krnnDiagram"));
}

ModelTreeItem *Model::addElementToModel( ModelTreeItem *parentItem, const Id &id,
		const QString &oldPathToItem, const QString &name, const QPointF &position, Qt::DropAction action )
{
	Q_UNUSED(oldPathToItem)
	Q_UNUSED(action)

	if (isDiagram(id))
	{
		if ((!isDiagram(parentItem->id())) && (parentItem != rootItem))
		{
			qDebug() << "Диаграмму нельзя добавить в элемент.";
			return NULL;
		}
	}
	else
	{
		if (parentItem == rootItem)
		{
			qDebug() << "Элемент можно добавить только на диаграмму";
			return NULL;
		}
	}

	int newRow = parentItem->children().size();
	beginInsertRows(index(parentItem),newRow,newRow);
		ModelTreeItem *item = new ModelTreeItem(id, parentItem);
		parentItem->addChild(item);
		treeItems.insert(id,item);
		mApi.addChild(parentItem->id(), id);
		mApi.setProperty(id, "name", name);
		mApi.setProperty(id, "from", ROOT_ID.toVariant());
		mApi.setProperty(id, "to", ROOT_ID.toVariant());
		mApi.setProperty(id, "fromPort", 0.0);
		mApi.setProperty(id, "toPort", 0.0);
		mApi.setProperty(id, "links", IdListHelper::toVariant(IdList()));
		mApi.setProperty(id, positionPropertyName(item), position);
		mApi.setProperty(id, configurationPropertyName(item), QVariant(QPolygon()));

		QStringList properties = mEditorManager.getPropertyNames(id.type());
		foreach (QString property, properties) {
			// Здесь должна быть инициализация значениями по умолчанию
			// (а ещё лучше, если не здесь). Считать этот код временным хаком,
			// пока нет системы типов.
			mApi.setProperty(id, property, "");
		}
	endInsertRows();
	return item;
}

void Model::loadSubtreeFromClient(ModelTreeItem * const parent)
{
	foreach (Id childId, mApi.children(parent->id())) {
		QString path = pathToItem(parent);
		ModelTreeItem *child = loadElement(parent, childId);
		loadSubtreeFromClient(child);
	}
}

ModelTreeItem *Model::loadElement(ModelTreeItem *parentItem, const Id &id)
{
	int newRow = parentItem->children().size();
	beginInsertRows(index(parentItem), newRow, newRow);
		ModelTreeItem *item = new ModelTreeItem(id, parentItem);
		parentItem->addChild(item);
		treeItems.insert(id, item);
	endInsertRows();
	return item;
}

QPersistentModelIndex Model::rootIndex()
{
	return index(rootItem);
}

QString Model::positionPropertyName(ModelTreeItem const *item) const
{
	return "position + " + pathToItem(item);
}

QString Model::configurationPropertyName(ModelTreeItem const *item) const
{
	return "configuration + " + pathToItem(item);
}

void Model::exterminate()
{
	mApi.exterminate();
	cleanupTree(rootItem);
	treeItems.clear();
	delete rootItem;
	rootItem = new ModelTreeItem(ROOT_ID, NULL);
	reset();
	init();
}

void Model::cleanupTree(ModelTreeItem *root)
{
	foreach (ModelTreeItem *childItem, root->children())
	{
		cleanupTree(childItem);
		delete childItem;
	}
	root->clearChildren();
}

qrRepo::RepoApi const & Model::api() const
{
	return mApi;
}
