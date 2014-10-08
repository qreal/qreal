#include "pasteEdgeCommand.h"
#include "mainwindow/mainWindow.h"
#include "umllib/private/reshapeEdgeCommand.h"

using namespace qReal::commands;

PasteEdgeCommand::PasteEdgeCommand(EditorViewScene *scene
		, EditorViewMViface const *mvIface
		, EdgeData const &data
		, QPointF const &offset
		, bool isGraphicalCopy
		, QHash<qReal::Id, qReal::Id> *copiedIds)
	: PasteCommand(scene, mvIface, offset, isGraphicalCopy, copiedIds)
	, mEdgeData(data), mCreateCommand(NULL)
{
}

Id PasteEdgeCommand::pasteNewInstance()
{
	// TODO: create it during initialization, not execution
	// TODO: create node/edge data hierarchy and move it into PasteCommand
	Id resultId = mResult;
	if (!mCreateCommand) {
		Id const typeId = mEdgeData.id.type();
		resultId = mScene->createElement(typeId.toString(), mEdgeData.pos + mOffset, true, &mCreateCommand, false);
		if (mCreateCommand) {
			mCreateCommand->redo();
			mCopiedIds->insert(mEdgeData.id, resultId);
			addPreAction(mCreateCommand);
		}
	}
	return resultId;
}

Id PasteEdgeCommand::pasteGraphicalCopy()
{
	Id resultId = mResult;
	if (!mCreateCommand) {
		mCreateCommand = new CreateElementCommand(
				*mMVIface->logicalAssistApi()
				, *mMVIface->graphicalAssistApi()
				, mScene->mainWindow()->exploser()
				, mMVIface->rootId()
				, mMVIface->rootId()
				, mEdgeData.logicalId
				, true
				, mMVIface->graphicalAssistApi()->name(mEdgeData.id)
				, mEdgeData.pos);

		mCreateCommand->redo();
		resultId = mCreateCommand->result();
		if (!resultId.isNull()) {
			mCopiedIds->insert(mEdgeData.id, resultId);
			addPreAction(mCreateCommand);
		}
	}

	EdgeElement * const newEdge = new EdgeElement(
			mScene->mainWindow()->editorManager().elementImpl(resultId)
			, resultId
			, *mMVIface->graphicalAssistApi()
			, *mMVIface->logicalAssistApi()
			);

	newEdge->setController(mScene->mainWindow()->controller());

	return resultId;
}

void PasteEdgeCommand::restoreElement()
{
	if (mResult.isNull()) {
		return;
	}

	Id const edgeId = mResult;

	Id const newSrcId = mCopiedIds->value(mEdgeData.srcId);
	Id const newDstId = mCopiedIds->value(mEdgeData.dstId);

	Id const logicalId = mMVIface->graphicalAssistApi()->logicalId(mCreateCommand->result());
	mMVIface->graphicalAssistApi()->setProperties(logicalId, mEdgeData.logicalProperties);

	mMVIface->graphicalAssistApi()->setPosition(edgeId, mEdgeData.pos + mOffset);
	mMVIface->graphicalAssistApi()->setConfiguration(edgeId, mEdgeData.configuration);

	mMVIface->graphicalAssistApi()->setFrom(edgeId, newSrcId.isNull() ? Id::rootId() : newSrcId);
	mMVIface->graphicalAssistApi()->setTo(edgeId, newDstId.isNull() ? Id::rootId() : newDstId);

	mMVIface->graphicalAssistApi()->setFromPort(edgeId, mEdgeData.portFrom);
	mMVIface->graphicalAssistApi()->setToPort(edgeId, mEdgeData.portTo);

	EdgeElement *edge = mScene->getEdgeById(edgeId);
	if (edge) {
		edge->changeShapeType(static_cast<enums::linkShape::LinkShape>(mEdgeData.shapeType));
		edge->setPos(mEdgeData.pos + mOffset);
		edge->setLine(mEdgeData.configuration);
		edge->connectToPort();
		edge->updateData();
		if (SettingsManager::value("ActivateGrid").toBool()) {
			edge->alignToGrid();
		}
	}
}
