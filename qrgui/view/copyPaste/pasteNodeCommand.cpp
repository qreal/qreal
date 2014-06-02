#include "pasteNodeCommand.h"

#include "mainwindow/mainWindow.h"

using namespace qReal::commands;

PasteNodeCommand::PasteNodeCommand(EditorViewScene *scene
		, EditorViewMViface const *mvIface
		, NodeData const &data
		, QPointF const &offset
		, bool isGraphicalCopy
		, QHash<Id, Id> *copiedIds)
	: PasteCommand(scene, mvIface, offset, isGraphicalCopy, copiedIds)
	, mNodeData(data)
	, mCreateCommand(NULL)
{
}

Id PasteNodeCommand::pasteNewInstance()
{
	// TODO: create it during initialization, not execution
	// TODO: create node/edge data hierarchy and move it into PasteCommand
	Id resultId = mResult;
	if (!mCreateCommand) {
		Id const typeId = mNodeData.id.type();
		resultId = mScene->createElement(typeId.toString(), newPos(), true, &mCreateCommand, false
				, vectorFromContainer());
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
	if (!mCreateCommand) {
		mCreateCommand = new CreateElementCommand(
			*mMVIface->logicalAssistApi()
			, *mMVIface->graphicalAssistApi()
			, mMVIface->rootId()
			, newGraphicalParent()
			, mNodeData.logicalId
			, true
			, mMVIface->graphicalAssistApi()->name(mNodeData.id)
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
	if (!mCreateCommand) {
		return;
	}

	Id const logicalId = mMVIface->graphicalAssistApi()->logicalId(mCreateCommand->result());
	mMVIface->graphicalAssistApi()->setProperties(logicalId, mNodeData.logicalProperties);
	mMVIface->graphicalAssistApi()->setProperties(mResult, mNodeData.graphicalProperties);
	mMVIface->graphicalAssistApi()->setPosition(mResult, newGraphicalPos());
	if (mCopiedIds->contains(mNodeData.parentId)) {
		mMVIface->graphicalAssistApi()->changeParent(mResult, mCopiedIds->value(mNodeData.parentId), newPos());
	}

	NodeElement *element = mScene->getNodeById(mResult);
	if (element) {
		element->updateData();
	}
}

QPointF PasteNodeCommand::newPos() const
{
	return mNodeData.pos + (mCopiedIds->contains(mNodeData.parentId) ?
			mMVIface->graphicalAssistApi()->position(mCopiedIds->value(mNodeData.parentId)) : mOffset);
}

QPointF PasteNodeCommand::newGraphicalPos() const
{
	return mNodeData.pos + (mCopiedIds->contains(mNodeData.parentId) ?
			QPointF() : mOffset);
}

Id PasteNodeCommand::newGraphicalParent() const
{
	return (mCopiedIds->contains(mNodeData.parentId) ?
			mCopiedIds->value(mNodeData.parentId) : mMVIface->rootId());
}

QPointF PasteNodeCommand::vectorFromContainer() const
{
	return (mNodeData.parentId == Id::rootId()) ? QPointF() : mNodeData.pos;
}
