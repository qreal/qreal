#include "logicalModel.h"

#include "graphicalModel.h"

using namespace qReal;
using namespace models;
using namespace details;

LogicalModel::LogicalModel(qrRepo::RepoApi &repoApi, EditorManager const &editorManager)
	: AbstractModel(repoApi, editorManager), mGraphicalModelView(this)
{
	mRootItem = new LogicalModelItem(Id::rootId(), NULL);
	mModelItems.insert(Id::rootId(), mRootItem);
}

void LogicalModel::connectToGraphicalModel(GraphicalModel * const graphicalModel)
{
	mGraphicalModelView.setModel(graphicalModel);
}

AbstractModelItem *LogicalModel::createModelItem(Id const &id, AbstractModelItem *parentItem) const
{
	return new LogicalModelItem(id, static_cast<LogicalModelItem *>(parentItem));
}

QModelIndex LogicalModel::indexById(Id const &id) const
{
	if (mModelItems.keys().contains(id)) {
		return index(mModelItems.find(id).value());
	}
	return QModelIndex();
}

void LogicalModel::updateElements(Id const &logicalId, QString const &name)
{
	if (!mNotNeedUpdate) {
		mNotNeedUpdate = true;
		return;
	}
	mApi.setName(logicalId, name);
	emit dataChanged(indexById(logicalId), indexById(logicalId));
}

QMimeData* LogicalModel::mimeData(QModelIndexList const &indexes) const
{
	QByteArray data;
	QDataStream stream(&data, QIODevice::WriteOnly);
	foreach (QModelIndex index, indexes) {
		if (index.isValid()) {
			AbstractModelItem *item = static_cast<AbstractModelItem*>(index.internalPointer());
			stream << item->id().toString();
			stream << pathToItem(item);
			stream << mApi.property(item->id(), "name").toString();
			stream << mApi.property(item->id(), "position").toPointF();
		} else {
			stream << QString();
			stream << Id::rootId().toString();
			stream << QString();
			stream << QPointF();
		}
	}

	QMimeData *mimeData = new QMimeData();
	mimeData->setData(DEFAULT_MIME_TYPE, data);
	return mimeData;
}

QString LogicalModel::pathToItem(AbstractModelItem const *item) const
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
		return Id::rootId().toString();
}

void LogicalModel::addElementToModel(const Id &parent, const Id &id, const Id &logicalId, const QString &name, const QPointF &position)
{
	Q_UNUSED(logicalId);
	if (mModelItems.contains(id))
		return;
	Q_ASSERT_X(mModelItems.contains(parent), "addElementToModel", "Adding element to non-existing parent");
	AbstractModelItem *parentItem = mModelItems[parent];
	int const newRow = parentItem->children().size();

	beginInsertRows(index(parentItem), newRow, newRow);
	AbstractModelItem *newItem = createModelItem(id, parentItem);
	parentItem->addChild(newItem);
	mApi.addChild(parentItem->id(), id);
	mApi.setProperty(id, "name", name);
	mApi.setProperty(id, "from", Id::rootId().toVariant());
	mApi.setProperty(id, "to", Id::rootId().toVariant());
	mApi.setProperty(id, "fromPort", 0.0);
	mApi.setProperty(id, "toPort", 0.0);
	mApi.setProperty(id, "links", IdListHelper::toVariant(IdList()));
	mApi.setProperty(id, "outgoingConnections", IdListHelper::toVariant(IdList()));
	mApi.setProperty(id, "incomingConnections", IdListHelper::toVariant(IdList()));
	mApi.setProperty(id, "outgoingUsages", IdListHelper::toVariant(IdList()));
	mApi.setProperty(id, "incomingUsages", IdListHelper::toVariant(IdList()));
	mApi.setProperty(id, "position", position);
	mApi.setProperty(id, "configuration", QVariant(QPolygon()));
	mModelItems.insert(id, newItem);
	endInsertRows();
}




