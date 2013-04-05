#include <QtGui/QApplication>
#include <QtGui/QClipboard>

#include "pasteGroupCommand.h"
#include "pasteNodeCommand.h"
#include "pasteEdgeCommand.h"

using namespace qReal::commands;

PasteGroupCommand::PasteGroupCommand(EditorViewScene *scene
		, EditorViewMViface const *mvIface
		, bool isGraphicalCopy)
	: mScene(scene), mMVIface(mvIface), mIsGraphicalCopy(isGraphicalCopy)
{
	prepareCommands();
}

PasteGroupCommand::~PasteGroupCommand()
{
	delete mCopiedIds;
}

void PasteGroupCommand::prepareCommands()
{
	QList<NodeData> nodesData;
	QList<EdgeData> edgesData;
	pullDataFromClipboard(nodesData, edgesData);

	if (nodesData.isEmpty()) {
		return;
	}

	QPointF const offset = mScene->getMousePos() - nodesData[0].pos;

	mCopiedIds = preparePasteNodesCommands(nodesData, offset);

	foreach (EdgeData const &data, edgesData) {
		preparePasteEdgeCommand(data, offset);
	}
}

QHash<Id, Id> *PasteGroupCommand::preparePasteNodesCommands(QList<NodeData> &nodesData
		, QPointF const &offset)
{
	QHash<Id, Id> *copiedIds = new QHash<Id, Id>();

	while (!nodesData.isEmpty()) {
		NodeData const &nextToPaste = nodesData[0];
		PasteNodeCommand *pasteCommand = new PasteNodeCommand(mScene, mMVIface
				, nextToPaste, offset, mIsGraphicalCopy, copiedIds);
		addPreAction(pasteCommand);
		nodesData.removeAll(nextToPaste);
	}

	return copiedIds;
}

void PasteGroupCommand::preparePasteEdgeCommand(EdgeData const &edgeData, QPointF const &offset)
{
	PasteEdgeCommand *pasteCommand = new PasteEdgeCommand(mScene, mMVIface
			, edgeData, offset, mIsGraphicalCopy, mCopiedIds);
	addPreAction(pasteCommand);
}

bool PasteGroupCommand::execute()
{
	return true;
}

bool PasteGroupCommand::restoreState()
{
	return true;
}

void PasteGroupCommand::pullDataFromClipboard(QList<NodeData> &nodesData, QList<EdgeData> &edgesData) const
{
	QClipboard const *clipboard = QApplication::clipboard();
	QMimeData const *mimeData = clipboard->mimeData();

	QByteArray data = mimeData->data("application/x-real-uml-model-data");
	QDataStream stream(&data, QIODevice::ReadOnly);

	stream >> nodesData;
	stream >> edgesData;
}
