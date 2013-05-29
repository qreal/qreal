#include <QtWidgets/QApplication>
#include <QtGui/QClipboard>

#include "pasteGroupCommand.h"
#include "pasteNodeCommand.h"
#include "pasteEdgeCommand.h"

using namespace qReal::commands;

PasteGroupCommand::PasteGroupCommand(EditorViewScene *scene
		, EditorViewMViface const *mvIface
		, bool isGraphicalCopy)
	: mScene(scene), mMVIface(mvIface)
	, mIsGraphicalCopy(isGraphicalCopy), mIsEmpty(false)
{
	prepareCommands();
}

PasteGroupCommand::~PasteGroupCommand()
{
}

bool PasteGroupCommand::isEmpty() const
{
	return mIsEmpty;
}

void PasteGroupCommand::prepareCommands()
{
	QList<NodeData> nodesData;
	QList<EdgeData> edgesData;
	pullDataFromClipboard(nodesData, edgesData);

	if (nodesData.isEmpty() && edgesData.isEmpty()) {
		mIsEmpty = true;
		return;
	}

	QPointF const originalPos = nodesData.isEmpty() ? edgesData[0].pos : nodesData[0].pos;
	QPointF const offset = mScene->getMousePos() - originalPos;

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
	// Execution happens in child commands
	return true;
}

bool PasteGroupCommand::restoreState()
{
	// Restoration happens in child commands
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
