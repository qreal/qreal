#include "pasteEdgeCommand.h"

#include <qrgui/models/models.h>

#include "editor/commands/reshapeEdgeCommand.h"

using namespace qReal::commands;

PasteEdgeCommand::PasteEdgeCommand(EditorViewScene *scene
		, EdgeData const &data
		, QPointF const &offset
		, bool isGraphicalCopy
		, QHash<qReal::Id, qReal::Id> *copiedIds)
	: PasteCommand(scene, offset, isGraphicalCopy, copiedIds)
	, mEdgeData(data)
	, mCreateCommand(nullptr)
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
				mScene->models().logicalModelAssistApi()
				, mScene->models().graphicalModelAssistApi()
				, mScene->models().exploser()
				, mScene->rootItemId()
				, mScene->rootItemId()
				, mEdgeData.logicalId
				, true
				, mScene->models().graphicalModelAssistApi().name(mEdgeData.id)
				, mEdgeData.pos);

		mCreateCommand->redo();
		resultId = mCreateCommand->result();
		if (!resultId.isNull()) {
			mCopiedIds->insert(mEdgeData.id, resultId);
			addPreAction(mCreateCommand);
		}
	}

	EdgeElement * const newEdge = new EdgeElement(
			mScene->models().logicalModelAssistApi().editorManagerInterface().elementImpl(resultId)
			, resultId
			, mScene->models().graphicalModelAssistApi()
			, mScene->models().logicalModelAssistApi()
			);

	newEdge->setController(&mScene->controller());

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

	Id const logicalId = mScene->models().graphicalModelAssistApi().logicalId(mCreateCommand->result());
	mScene->models().graphicalModelAssistApi().setProperties(logicalId, mEdgeData.logicalProperties);

	mScene->models().graphicalModelAssistApi().setPosition(edgeId, mEdgeData.pos + mOffset);
	mScene->models().graphicalModelAssistApi().setConfiguration(edgeId, mEdgeData.configuration);

	mScene->models().graphicalModelAssistApi().setFrom(edgeId, newSrcId.isNull() ? Id::rootId() : newSrcId);
	mScene->models().graphicalModelAssistApi().setTo(edgeId, newDstId.isNull() ? Id::rootId() : newDstId);

	mScene->models().graphicalModelAssistApi().setFromPort(edgeId, mEdgeData.portFrom);
	mScene->models().graphicalModelAssistApi().setToPort(edgeId, mEdgeData.portTo);

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
