#include "logicalModel.h"

#include "graphicalModel.h"

#include<QtCore/QDebug>

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
			stream << mApi.property(item->id(), "name");
			stream << mApi.property(item->id(), "position").toPointF();
		} else {
			stream << Id::rootId().toString();
			stream << QString();
			stream << Id::rootId().toString();
			stream << QPointF();
		}
	}
	QMimeData *mimeData = new QMimeData();
	mimeData->setData(DEFAULT_MIME_TYPE, data);
	return mimeData;
}


