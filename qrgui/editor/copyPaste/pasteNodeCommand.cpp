#include "pasteNodeCommand.h"

#include <qrgui/models/models.h>

using namespace qReal::commands;

PasteNodeCommand::PasteNodeCommand(EditorViewScene *scene
		, NodeData const &data
		, const QPointF &offset
		, bool isGraphicalCopy
		, QHash<Id, Id> *copiedIds)
	: PasteCommand(scene, offset, isGraphicalCopy, copiedIds)
	, mNodeData(data)
	, mCreateCommand(nullptr)
{
}

Id PasteNodeCommand::pasteNewInstance()
{
	// TODO: create it during initialization, not execution
	// TODO: create node/edge data hierarchy and move it into PasteCommand
	Id resultId = mResult;
	if (!mCreateCommand && explosionTargetExists()) {
		const Id typeId = mNodeData.id.type();
		resultId = mScene->createElement(typeId.toString(), newPos(), true, &mCreateCommand, false
				, vectorFromContainer(), mNodeData.explosion.toString());
		if (mCreateCommand) {
			mCreateCommand->redo();
			mCopiedIds->insert(mNodeData.id, resultId);
			addPreAction(mCreateCommand);
		}
	}
	return resultId;
}

Id PasteNodeCommand::pasteGraphicalCopy()
{
	Id resultId = mResult;
	if (!mCreateCommand && explosionTargetExists()) {
		mCreateCommand = new CreateElementCommand(
			mScene->models().logicalModelAssistApi()
			, mScene->models().graphicalModelAssistApi()
			, mScene->models().exploser()
			, mScene->rootItemId()
			, newGraphicalParent()
			, mNodeData.logicalId
			, true
			, mScene->models().graphicalModelAssistApi().name(mNodeData.id)
			, newGraphicalPos()
			);

		mCreateCommand->redo();
		resultId = mCreateCommand->result();
		if (!resultId.isNull()) {
			mCopiedIds->insert(mNodeData.id, resultId);
			addPreAction(mCreateCommand);
		}
	}

	return resultId;
}

void PasteNodeCommand::restoreElement()
{
	if (!mCreateCommand || !explosionTargetExists()) {
		return;
	}

	const Id logicalId = mScene->models().graphicalModelAssistApi().logicalId(mCreateCommand->result());
	mScene->models().graphicalModelAssistApi().setProperties(logicalId, mNodeData.logicalProperties);
	mScene->models().graphicalModelAssistApi().setProperties(mResult, mNodeData.graphicalProperties);
	mScene->models().graphicalModelAssistApi().setPosition(mResult, newGraphicalPos());
	if (mCopiedIds->contains(mNodeData.parentId)) {
		mScene->models().graphicalModelAssistApi().changeParent(mResult, mCopiedIds->value(mNodeData.parentId), newPos());
	}

	NodeElement *element = mScene->getNodeById(mResult);
	if (element) {
		element->updateData();
	}
}

bool PasteNodeCommand::explosionTargetExists() const
{
	return mNodeData.explosion.isNull() || mScene->models().logicalRepoApi().exist(mNodeData.explosion);
}

QPointF PasteNodeCommand::newPos() const
{
	return mNodeData.pos + (mCopiedIds->contains(mNodeData.parentId) ?
			mScene->models().graphicalModelAssistApi().position(mCopiedIds->value(mNodeData.parentId)) : mOffset);
}

QPointF PasteNodeCommand::newGraphicalPos() const
{
	return mNodeData.pos + (mCopiedIds->contains(mNodeData.parentId) ?
			QPointF() : mOffset);
}

Id PasteNodeCommand::newGraphicalParent() const
{
	return (mCopiedIds->contains(mNodeData.parentId) ?
			mCopiedIds->value(mNodeData.parentId) : mScene->rootItemId());
}

QPointF PasteNodeCommand::vectorFromContainer() const
{
	return (mNodeData.parentId == Id::rootId()) ? QPointF() : mNodeData.pos;
}
