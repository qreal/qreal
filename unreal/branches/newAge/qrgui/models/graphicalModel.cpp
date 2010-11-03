#include "graphicalModel.h"

#include "logicalModel.h"

#include <QtCore/QUuid>

using namespace qReal;
using namespace models;
using namespace details;

GraphicalModel::GraphicalModel(qrRepo::RepoApi &repoApi, const EditorManager &editorManager)
	: AbstractModel(repoApi, editorManager), mLogicalModelView(this)
{
	mRootItem = new GraphicalModelItem(Id::rootId(), Id(), NULL);
	mModelItems.insert(Id::rootId(), mRootItem);
}

void GraphicalModel::connectToLogicalModel(LogicalModel * const logicalModel)
{
	mLogicalModelView.setModel(logicalModel);
}

QVariant GraphicalModel::data(const QModelIndex &index, int role) const
{
	if (index.isValid()) {
		GraphicalModelItem *item = static_cast<GraphicalModelItem*>(index.internalPointer());
		Q_ASSERT(item);
		switch (role) {
		case Qt::DisplayRole:
		case Qt::EditRole:
			return mApi.name(item->id());
		case Qt::DecorationRole:
			return QVariant();
			// return mEditorManager.icon(item->id());
		case roles::idRole:
			return item->id().toVariant();
		case roles::logicalIdRole:
			return item->logicalId().toVariant();
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

AbstractModelItem *GraphicalModel::createModelItem(Id const &id, AbstractModelItem *parentItem) const
{
	return new GraphicalModelItem(id, Id(id.type(), QUuid::createUuid().toString())
								  , static_cast<GraphicalModelItem *>(parentItem));
}

void GraphicalModel::updateElements(Id const &logicalId, QString const &name)
{
	if (!mNotNeedUpdate) {
		mNotNeedUpdate = true;
		return;
	}
	foreach (AbstractModelItem *item,  mModelItems.values()) {
		GraphicalModelItem *graphicalItem = static_cast<GraphicalModelItem *>(item);
		if (graphicalItem->logicalId() == logicalId) {
			mApi.setName(graphicalItem->id(), name);
			emit dataChanged(index(graphicalItem), index(graphicalItem));
		}
	}
}
