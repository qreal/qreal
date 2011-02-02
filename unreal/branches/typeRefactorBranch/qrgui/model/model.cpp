#include "model.h"
#include <QtGui/QIcon>
#include <QtGui/QPolygon>
#include <QtCore/QDebug>
#include <QtCore/QUuid>

using namespace qReal;
using namespace model;
using namespace details;

Model::Model(EditorManager const &editorManager, QString const &workingDirectory)
	:  mApi(workingDirectory), mEditorManager(editorManager), mAssistApi(*this, editorManager), mLogging(false)
{
	mRootItem = new ModelTreeItem(ROOT_ID, NULL);
	init();
}

Model::~Model()
{
	cleanupTree(mRootItem);
	mTreeItems.clear();
}

bool Model::isDiagram(NewType const &type) const
{
        return ((type != ROOT_ID) && (type.element() == assistApi().editorManager().getEditorInterface(type.editor())->diagramNodeName(type.diagram())));
}

details::ModelTreeItem* Model::isSituatedOn(details::ModelTreeItem *element) const
{
	while(element->parent() != mRootItem)
		element = element->parent();
	return element;
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
	mLogging = true;
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
                                return mApi.name(item->type());
			case Qt::DecorationRole:
                                return mEditorManager.icon(item->type());
			case roles::idRole:
                                return item->type().toVariant();
			case roles::positionRole:
                                return mApi.property(item->type(), "position");
			case roles::fromRole:
                                return mApi.from(item->type()).toVariant();
			case roles::toRole:
                                return mApi.to(item->type()).toVariant();
			case roles::fromPortRole:
                                return mApi.fromPort(item->type());
			case roles::toPortRole:
                                return mApi.toPort(item->type());
			case roles::configurationRole:
                                return mApi.property(item->type(), "configuration");
		}
		if (role >= roles::customPropertiesBeginRole) {
                        QString selectedProperty = findPropertyName(item->type(), role);
                        return mApi.property(item->type(), selectedProperty);
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
                NewType type = item->type();
                QString message = QString("Data changed:\n"+type.toString()+"\n");
		switch (role) {
		case Qt::DisplayRole:
		case Qt::EditRole:
                        mApi.setName(type, value.toString());
			message += "Set Name:\n";
			emit nameChanged(index);
			break;
		case roles::positionRole:
                        mApi.setProperty(type, "position", value);
			message += "Set Position:\n";
			break;
		case roles::configurationRole:
                        mApi.setProperty(type, "configuration", value);
			message += "Set Configuration:\n";
			break;
		case roles::fromRole:
                        mApi.setFrom(type, value.value<NewType>());
			message += "Set From:\n";
			break;
		case roles::toRole:
                        mApi.setTo(type, value.value<NewType>());
			message += "Set To:\n";
			break;
		case roles::fromPortRole:
                        mApi.setFromPort(type, value.toDouble());
			message += "Set FromPort:\n";
			break;
		case roles::toPortRole:
                        mApi.setToPort(type, value.toDouble());
			message += "Set ToPort:\n";
			break;
		default:
			if (role >= roles::customPropertiesBeginRole) {
                                QString selectedProperty = findPropertyName(type, role);
                                mApi.setProperty(type, selectedProperty, value);
				message += "Set " + selectedProperty + "\n";
				break;
			}
			Q_ASSERT(role < Qt::UserRole);
			return false;
		}

		QString output;
		QDebug qD = QDebug(&output);
		qD << value;
		message += output + "\n";
                log(message, isSituatedOn(item)->type());

		emit dataChanged(index, index);
		return true;
	}
	return false;
}

QString Model::findPropertyName(NewType const &type, int const role) const
{
	// В случае свойства, описанного в самом элементе, роль - просто
	// порядковый номер свойства в списке свойств. Этого соглашения
	// надо всюду придерживаться, а то роли "поедут".
        QStringList properties = mEditorManager.getPropertyNames(type);
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
                QString selectedProperty = findPropertyName(item->type(), role);
                return mEditorManager.getEnumValues(item->type(), selectedProperty);
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
                QString selectedProperty = findPropertyName(item->type(), role);
                return mEditorManager.getTypeName(item->type(), selectedProperty);
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
                        mApi.removeElement(child->type());
			removeModelItems(child);

			// TODO: Убрать копипасту.
			int childRow = child->row();
			beginRemoveRows(parent, childRow, childRow);
			child->parent()->removeChild(child);
                        mTreeItems.remove(child->type(), child);
                        if (mTreeItems.count(child->type()) == 0)
                                mApi.removeChild(parentItem->type(), child->type());
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
                        path = item->type().toString() + ID_PATH_DIVIDER + path;
		} while (item != mRootItem);
		return path;
	}
	else
		return ROOT_ID.toString();
}

void Model::removeConfigurationInClient(ModelTreeItem const * const item)
{
        mApi.removeProperty(item->type(), "position");
        mApi.removeProperty(item->type(), "configuration");
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
                mApi.removeElement(child->type());
		removeModelItems(child);
		int childRow = child->row();
		beginRemoveRows(index(root),childRow,childRow);
		removeConfigurationInClient(child);
		child->parent()->removeChild(child);
                mTreeItems.remove(child->type(),child);
                if (mTreeItems.count(child->type())==0) {
                        mApi.removeChild(root->type(),child->type());
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
                        stream << item->type().toString();
			stream << pathToItem(item);
                        stream << mApi.property(item->type(), "name");
                        stream << mApi.property(item->type(), "position").toPointF();
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
                QString typeString;
		QString pathToItem;
		QString name;
		QPointF position;
                stream >> typeString;
		stream >> pathToItem;
		stream >> name;
		stream >> position;
                NewType type = NewType::loadFromString(typeString);
                Q_ASSERT(type.typeSize() == 4);  // Бросать в модель мы можем только конкретные элементы.

                if (mTreeItems.contains(type))  // Пока на диаграмме не может быть больше одного экземпляра
			// одной и той же сущности, бросать существующие элементы нельзя.
			return false;

                return addElementToModel(parentItem, type, pathToItem, name, position, action) != NULL;
	}
}

ModelTreeItem *Model::addElementToModel(ModelTreeItem *parentItem, NewType const &type,
	QString const &oldPathToItem, QString const &name, QPointF const &position, Qt::DropAction action)
{
	Q_UNUSED(oldPathToItem)
	Q_UNUSED(action)

        if (isDiagram(type)) {
                if (!isDiagram(parentItem->type()) && parentItem != mRootItem) {
			qDebug() << "Diagram cannot be placed into element.";
			return NULL;
		}
		if (parentItem == mRootItem)
                        log("Creating diagram:\n" + type.toString() + "\n", type);
		else
                        log("Adding element:\n" + type.toString() + "\n", isSituatedOn(parentItem)->type());
	}
	else {
		if (parentItem == mRootItem) {
			qDebug() << "Element can be placed only on diagram.";
			return NULL;
		}
                log("Adding element:\n" + type.toString() + "\n", isSituatedOn(parentItem)->type());
	}

	int newRow = parentItem->children().size();
	beginInsertRows(index(parentItem), newRow, newRow);
                ModelTreeItem *item = new ModelTreeItem(type, parentItem);
		parentItem->addChild(item);
                mTreeItems.insert(type, item);
                mApi.addChild(parentItem->type(), type);
                mApi.setProperty(type, "name", name);
                mApi.setProperty(type, "from", ROOT_ID.toVariant());
                mApi.setProperty(type, "to", ROOT_ID.toVariant());
                mApi.setProperty(type, "fromPort", 0.0);
                mApi.setProperty(type, "toPort", 0.0);
                mApi.setProperty(type, "links", TypeListHelper::toVariant(TypeList()));
                mApi.setProperty(type, "outgoingConnections", TypeListHelper::toVariant(TypeList()));
                mApi.setProperty(type, "incomingConnections", TypeListHelper::toVariant(TypeList()));
                mApi.setProperty(type, "outgoingUsages", TypeListHelper::toVariant(TypeList()));
                mApi.setProperty(type, "incomingUsages", TypeListHelper::toVariant(TypeList()));
                mApi.setProperty(type, "position", position);
                mApi.setProperty(type, "configuration", QVariant(QPolygon()));

                QStringList properties = mEditorManager.getPropertyNames(type);
		foreach (QString property, properties)
		// for those properties that doesn't have default values, plugin will return empty string
                        mApi.setProperty(type, property, mEditorManager.getDefaultPropertyValue(type, property));

		endInsertRows();
	return item;
}

bool Model::addElementToModel(NewType const &parent, NewType const &type, QString const &name, QPointF const &position)
{
	ModelTreeItem *parentItem = parent == ROOT_ID ? mRootItem
                : static_cast<ModelTreeItem*>(indexByType(parent).internalPointer());
	Q_ASSERT(parentItem != NULL);
        return addElementToModel(parentItem, type, "", name, position, Qt::CopyAction) != NULL;
}

void Model::changeParent(QModelIndex const &element, QModelIndex const &parent, QPointF const &position)
{
	if (!parent.isValid() || element.parent() == parent)
		return;

	int destinationRow = parentTreeItem(parent)->children().size();

	if (beginMoveRows(element.parent(), element.row(), element.row(), parent, destinationRow)) {
		ModelTreeItem *elementItem = static_cast<ModelTreeItem*>(element.internalPointer());
                QVariant configuration = mApi.property(elementItem->type(), "configuration");
		elementItem->parent()->removeChild(elementItem);
		ModelTreeItem *parentItem = parentTreeItem(parent);

                mApi.addParent(elementItem->type(), parentItem->type());
                mApi.removeParent(elementItem->type(), elementItem->parent()->type());

		elementItem->setParent(parentItem);
		parentItem->addChild(elementItem);

                mApi.setProperty(elementItem->type(), "position", position);
                mApi.setProperty(elementItem->type(), "configuration", configuration);
		endMoveRows();
	}
}

void Model::loadSubtreeFromClient(ModelTreeItem * const parent)
{
        foreach (NewType childType, mApi.children(parent->type())) {
                ModelTreeItem *child = loadElement(parent, childType);
		loadSubtreeFromClient(child);
	}
}

ModelTreeItem *Model::loadElement(ModelTreeItem *parentItem, NewType const &type)
{
        if (isDiagram(type)) {
                        mApi.addOpenedDiagram(type);
                        qDebug() << type.toString();
		}

	int newRow = parentItem->children().size();
	beginInsertRows(index(parentItem), newRow, newRow);
                ModelTreeItem *item = new ModelTreeItem(type, parentItem);
                checkProperties(type);
		parentItem->addChild(item);
                mTreeItems.insert(type, item);
	endInsertRows();
	return item;
}

void Model::checkProperties(NewType const &type)
{
        if (!mEditorManager.hasElement(type))
		return;
        QStringList propertiesThatShallBe = mEditorManager.getPropertyNames(type);
	foreach (QString property, propertiesThatShallBe)
                if (!api().hasProperty(type, property))
                        mApi.setProperty(type, property, "");  // Типа значение по умолчанию.
        if (!api().hasProperty(type, "outgoingUsages"))
                mApi.setProperty(type, "outgoingUsages", TypeListHelper::toVariant(TypeList()));
        if (!api().hasProperty(type, "incomingUsages"))
                mApi.setProperty(type, "incomingUsages", TypeListHelper::toVariant(TypeList()));
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

void Model::addDiagram(NewType const &type)
{
        mApi.addChild(ROOT_ID, type);
}

void Model::resetChangedDiagrams()
{
	mApi.resetChangedDiagrams();
}

void Model::resetChangedDiagrams(const TypeList &list)
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

QModelIndex Model::indexByType(NewType const &type) const
{
        if (mTreeItems.keys().contains(type)) {
                return index(mTreeItems.find(type).value());
	}
	return QModelIndex();
}

NewType Model::typeByIndex(QModelIndex const &index) const
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

NewType Model::getRootDiagram()
{
        return mRootIndex.data(roles::idRole).value<NewType>();
}

void Model::setRootIndex(const QModelIndex &index)
{
	mRootIndex = index;
}

bool Model::isChanged()
{
	return (mApi.getChangedDiagrams().size() > 0);
}

void Model::log(QString const message, NewType const diagram)
{
	if (!mLogging)
		return;
	mApi.log(message, diagram);
}
