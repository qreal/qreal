#include "model.h"
#include <QtGui/QIcon>
#include <QtGui/QPolygon>
#include <QtCore/QDebug>
#include <QtCore/QUuid>

using namespace qReal;
using namespace model;
using namespace details;

Model::Model(EditorManager const &editorManager, QString const &workingDirectory)
	:  mApi(workingDirectory), mEditorManager(editorManager), mAssistApi(*this, editorManager)
{
	mRootItem = new ModelTreeItem(ROOT_ID, NULL);
	init();
}

Model::~Model()
{
	cleanupTree(mRootItem);
	mTreeItems.clear();
}

bool Model::isDiagram(Id const &id) const
{
	return ((id != ROOT_ID) && (id.element() == assistApi().editorManager().getEditorInterface(id.editor())->diagramNodeName(id.diagram())));
}

void Model::init()
{
	mTreeItems.insert(ROOT_ID, mRootItem);
	mApi.setName(ROOT_ID, ROOT_ID.toString());

	// Turn off view notification while loading. Model can be inconsistent during a process,
	// so views shall not update themselves before time. It is important for
	// scene, where adding edge before adding nodes may lead to disconnected edge.
	blockSignals(true);
	loadSubtreeFromClient(mRootItem);
	blockSignals(false);
	mApi.resetChangedDiagrams();
}

Qt::ItemFlags Model::flags(QModelIndex const &index) const
{
	if (index.isValid()) {
		return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled
			| Qt::ItemIsDropEnabled | Qt::ItemIsEnabled;
	} else {
	// root item has invalid index, but we should still be able to drop elements into it
		return Qt::ItemIsDropEnabled;
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

bool Model::setData(QModelIndex const &index, QVariant const &value, int role)
{
	qDebug() << value;
	if (index.isValid()) {
		qDebug() << value << role;
		ModelTreeItem *item = static_cast<ModelTreeItem*>(index.internalPointer());
		switch (role) {
		case Qt::DisplayRole:
		case Qt::EditRole:
			mApi.setName(item->id(), value.toString());
			emit nameChanged(index);
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

QStringList Model::getEnumValues(QModelIndex const &index, int const role) const
{
	do {
		if (!index.isValid())
			break;
		ModelTreeItem *item = static_cast<ModelTreeItem*>(index.internalPointer());
		if (!item)
			break;
		QString selectedProperty = findPropertyName(item->id(), role);
		return mEditorManager.getEnumValues(item->id(), selectedProperty);
	} while (false);

	return QStringList();
}

QString Model::getTypeName(QModelIndex const &index, int const role) const
{
	do {
		if (!index.isValid())
			break;
		ModelTreeItem *item = static_cast<ModelTreeItem*>(index.internalPointer());
		if (!item)
			break;
		QString selectedProperty = findPropertyName(item->id(), role);
		return mEditorManager.getTypeName(item->id(), selectedProperty);
	} while (false);

	return QString();
}

QVariant Model::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 0)
		return QVariant("name");
	else
		return QVariant();
}

int Model::rowCount(QModelIndex const &parent) const
{
	return parentTreeItem(parent)->children().size();
}

int Model::columnCount(QModelIndex const &parent) const
{
	Q_UNUSED(parent)
	return 1;
}

bool Model::removeRows(int row, int count, QModelIndex const &parent)
{
	ModelTreeItem *parentItem = parentTreeItem(parent);

	if (parentItem->children().size() < row + count)
		return false;
	else {
		for (int i = row; i < row + count; ++i) {
			ModelTreeItem *child = parentItem->children().at(i);
			mApi.removeElement(child->id());
			removeModelItems(child);

			// TODO: Убрать копипасту.
			int childRow = child->row();
			beginRemoveRows(parent, childRow, childRow);
			child->parent()->removeChild(child);
			mTreeItems.remove(child->id(), child);
			if (mTreeItems.count(child->id()) == 0)
				mApi.removeChild(parentItem->id(), child->id());
			delete child;
			endRemoveRows();
		}
		return true;
	}
}

QString Model::pathToItem(ModelTreeItem const *item) const
{
	if (item != mRootItem) {
		QString path;
		do {
			item = item->parent();
			path = item->id().toString() + ID_PATH_DIVIDER + path;
		} while (item != mRootItem);
		return path;
	}
	else
		return ROOT_ID.toString();
}

void Model::removeConfigurationInClient(ModelTreeItem const * const item)
{
	mApi.removeProperty(item->id(), "position");
	mApi.removeProperty(item->id(), "configuration");
}

QModelIndex Model::index(ModelTreeItem const * const item) const
{
	QList<int> rowCoords;

	for (ModelTreeItem const *curItem = item;
		curItem != mRootItem; curItem = curItem->parent())
	{
		rowCoords.append(const_cast<ModelTreeItem *>(curItem)->row());
	}

	QModelIndex result;

	for (int i = rowCoords.size() - 1; i >= 0; --i) {
		result = index(rowCoords[i], 0, result);
	}

	return result;
}

void Model::removeModelItems(ModelTreeItem *root)
{
	foreach (ModelTreeItem *child, root->children()) {
		mApi.removeElement(child->id());
		removeModelItems(child);
		int childRow = child->row();
		beginRemoveRows(index(root),childRow,childRow);
		removeConfigurationInClient(child);
		child->parent()->removeChild(child);
		mTreeItems.remove(child->id(),child);
		if (mTreeItems.count(child->id())==0) {
			mApi.removeChild(root->id(),child->id());
		}
		delete child;
		endRemoveRows();
	}
}

QModelIndex Model::index(int row, int column, QModelIndex const &parent) const
{
	ModelTreeItem *parentItem = parentTreeItem(parent);
	if (parentItem->children().size()<=row)
		return QModelIndex();
	ModelTreeItem *item = parentItem->children().at(row);
	return createIndex(row,column,item);
}

QModelIndex Model::parent(QModelIndex const &index) const
{
	if (index.isValid()) {
		ModelTreeItem *item = static_cast<ModelTreeItem*>(index.internalPointer());
		ModelTreeItem *parentItem = item->parent();
		if (parentItem == mRootItem || parentItem == NULL)
			return QModelIndex();
		else
			return createIndex(parentItem->row(), 0, parentItem);
	} else
		return QModelIndex();
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

QMimeData* Model::mimeData(QModelIndexList const &indexes) const
{
	QByteArray data;
	QDataStream stream(&data, QIODevice::WriteOnly);
	foreach (QModelIndex index, indexes) {
		if (index.isValid()) {
			ModelTreeItem *item = static_cast<ModelTreeItem*>(index.internalPointer());
			stream << item->id().toString();
			stream << pathToItem(item);
			stream << mApi.property(item->id(), "name");
			stream << mApi.property(item->id(), "position").toPointF();
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

bool Model::dropMimeData(QMimeData const *data, Qt::DropAction action, int row, int column, QModelIndex const &parent)
{
	Q_UNUSED(row)
	Q_UNUSED(column)
	if (action == Qt::IgnoreAction)
		return true;
	else {
		ModelTreeItem *parentItem = parentTreeItem(parent);

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

		if (mTreeItems.contains(id))  // Пока на диаграмме не может быть больше одного экземпляра
			// одной и той же сущности, бросать существующие элементы нельзя.
			return false;

		return addElementToModel(parentItem, id, pathToItem, name, position, action) != NULL;
	}
}

ModelTreeItem *Model::addElementToModel(ModelTreeItem *parentItem, Id const &id,
	QString const &oldPathToItem, QString const &name, QPointF const &position, Qt::DropAction action)
{
	Q_UNUSED(oldPathToItem)
	Q_UNUSED(action)

	if (isDiagram(id)) {
		if (!isDiagram(parentItem->id()) && parentItem != mRootItem) {
			qDebug() << "Diagram cannot be placed into element.";
			return NULL;
		}
	}
	else {
		if (parentItem == mRootItem) {
			qDebug() << "Element can be placed only on diagram.";
			return NULL;
		}
	}

	int newRow = parentItem->children().size();
	beginInsertRows(index(parentItem), newRow, newRow);
		ModelTreeItem *item = new ModelTreeItem(id, parentItem);
		parentItem->addChild(item);
		mTreeItems.insert(id, item);
		mApi.addChild(parentItem->id(), id);
		mApi.setProperty(id, "name", name);
		mApi.setProperty(id, "from", ROOT_ID.toVariant());
		mApi.setProperty(id, "to", ROOT_ID.toVariant());
		mApi.setProperty(id, "fromPort", 0.0);
		mApi.setProperty(id, "toPort", 0.0);
		mApi.setProperty(id, "links", IdListHelper::toVariant(IdList()));
		mApi.setProperty(id, "outgoingConnections", IdListHelper::toVariant(IdList()));
		mApi.setProperty(id, "incomingConnections", IdListHelper::toVariant(IdList()));
		mApi.setProperty(id, "outgoingUsages", IdListHelper::toVariant(IdList()));
		mApi.setProperty(id, "incomingUsages", IdListHelper::toVariant(IdList()));
		mApi.setProperty(id, "position", position);
		mApi.setProperty(id, "configuration", QVariant(QPolygon()));

		QStringList properties = mEditorManager.getPropertyNames(id.type());
		foreach (QString property, properties)
		// for those properties that doesn't have default values, plugin will return empty string
			mApi.setProperty(id, property, mEditorManager.getDefaultPropertyValue(id, property));

		endInsertRows();
	return item;
}

bool Model::addElementToModel(Id const &parent, Id const &id, QString const &name, QPointF const &position)
{
	ModelTreeItem *parentItem = parent == ROOT_ID ? mRootItem
		: static_cast<ModelTreeItem*>(indexById(parent).internalPointer());
	Q_ASSERT(parentItem != NULL);
	return addElementToModel(parentItem, id, "", name, position, Qt::CopyAction) != NULL;
}

void Model::changeParent(QModelIndex const &element, QModelIndex const &parent, QPointF const &position)
{
	if (!parent.isValid() || element.parent() == parent)
		return;

	int destinationRow = parentTreeItem(parent)->children().size();

	if (beginMoveRows(element.parent(), element.row(), element.row(), parent, destinationRow)) {
		ModelTreeItem *elementItem = static_cast<ModelTreeItem*>(element.internalPointer());
		QVariant configuration = mApi.property(elementItem->id(), "configuration");
		elementItem->parent()->removeChild(elementItem);
		ModelTreeItem *parentItem = parentTreeItem(parent);

		mApi.addParent(elementItem->id(), parentItem->id());
		mApi.removeParent(elementItem->id(), elementItem->parent()->id());

		elementItem->setParent(parentItem);
		parentItem->addChild(elementItem);

		mApi.setProperty(elementItem->id(), "position", position);
		mApi.setProperty(elementItem->id(), "configuration", configuration);
		endMoveRows();
	}
}

void Model::loadSubtreeFromClient(ModelTreeItem * const parent)
{
	foreach (Id childId, mApi.children(parent->id())) {
		ModelTreeItem *child = loadElement(parent, childId);
		loadSubtreeFromClient(child);
	}
}

ModelTreeItem *Model::loadElement(ModelTreeItem *parentItem, Id const &id)
{
	if (isDiagram(id)) {
			mApi.addOpenedDiagram(id);
			qDebug() << id.toString();
		}

	int newRow = parentItem->children().size();
	beginInsertRows(index(parentItem), newRow, newRow);
		ModelTreeItem *item = new ModelTreeItem(id, parentItem);
		checkProperties(id);
		parentItem->addChild(item);
		mTreeItems.insert(id, item);
	endInsertRows();
	return item;
}

void Model::checkProperties(Id const &id)
{
	if (!mEditorManager.hasElement(id.type()))
		return;
	QStringList propertiesThatShallBe = mEditorManager.getPropertyNames(id.type());
	foreach (QString property, propertiesThatShallBe)
		if (!api().hasProperty(id, property))
			mApi.setProperty(id, property, "");  // Типа значение по умолчанию.
	if (!api().hasProperty(id, "outgoingUsages"))
		mApi.setProperty(id, "outgoingUsages", IdListHelper::toVariant(IdList()));
	if (!api().hasProperty(id, "incomingUsages"))
		mApi.setProperty(id, "incomingUsages", IdListHelper::toVariant(IdList()));
}

QPersistentModelIndex Model::rootIndex() const
{
	return index(mRootItem);
}

void Model::open(QString const &workingDirectory)
{
	mApi.open(workingDirectory);
	reinit();
}

void Model::saveTo(QString const &workingDirectory)
{
	mApi.saveTo(workingDirectory);
}

void Model::save()
{
	mApi.saveAll();
	mApi.resetChangedDiagrams();
}

void Model::reinit()
{
	cleanupTree(mRootItem);
	mTreeItems.clear();
	delete mRootItem;
	mRootItem = new ModelTreeItem(ROOT_ID, NULL);
	reset();
	init();
}

void Model::exterminate()
{
	mApi.exterminate();
	reinit();
}

void Model::addDiagram(Id const &id)
{
	mApi.addChild(ROOT_ID, id);
}

void Model::resetChangedDiagrams()
{
	mApi.resetChangedDiagrams();
}

void Model::resetChangedDiagrams(const IdList &list)
{
	mApi.resetChangedDiagrams(list);
}


void Model::cleanupTree(ModelTreeItem *root)
{
	foreach (ModelTreeItem *childItem, root->children()) {
		cleanupTree(childItem);
		delete childItem;
	}
	root->clearChildren();
}

qrRepo::RepoApi const & Model::api() const
{
	return mApi;
}

ModelTreeItem *Model::parentTreeItem(QModelIndex const &parent) const
{
	return parent.isValid()
		? static_cast<ModelTreeItem*>(parent.internalPointer())
		: mRootItem
	;
}

qrRepo::RepoApi& Model::mutableApi()
{
	return mApi;
}

QModelIndex Model::indexById(Id const &id) const
{
	if (mTreeItems.keys().contains(id)) {
		return index(mTreeItems.find(id).value());
	}
	return QModelIndex();
}

Id Model::idByIndex(QModelIndex const &index) const
{
	ModelTreeItem *item = static_cast<ModelTreeItem*>(index.internalPointer());
	return mTreeItems.key(item);
}

ModelAssistApi &Model::assistApi()
{
	return mAssistApi;
}

ModelAssistApi const &Model::assistApi() const
{
	return mAssistApi;
}

Id Model::getRootDiagram()
{
	return mRootIndex.data(roles::idRole).value<Id>();
}

void Model::setRootIndex(const QModelIndex &index)
{
	mRootIndex = index;
}

bool Model::isChanged()
{
	return (mApi.getChangedDiagrams().size() > 0);
}
