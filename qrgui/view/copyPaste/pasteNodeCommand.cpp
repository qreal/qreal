#include "pasteNodeCommand.h"
#include "../../mainwindow/mainWindow.h"

using namespace qReal::commands;

PasteNodeCommand::PasteNodeCommand(EditorViewScene *scene
		, EditorViewMViface const *mvIface
		, NodeData const &data
		, QPointF const &offset
		, bool isGraphicalCopy
		, QHash<Id, Id> *copiedIds)
	: PasteCommand(scene, mvIface, offset, isGraphicalCopy, copiedIds)
	, mNodeData(data), mNewPos(getNewPos()), mCreateCommand(NULL)
{
}

Id PasteNodeCommand::pasteNewInstance()
{
	// TODO: create it during initialization, not execution
	// TODO: create node/edge data hierarchy and move it into PasteCommand
	Id resultId = mResult;
	if (!mCreateCommand) {
		Id const typeId = mNodeData.id.type();
		resultId = mScene->createElement(typeId.toString(), mNewPos, true, &mCreateCommand, false);
		mCreateCommand->redo();
		mCopiedIds->insert(mNodeData.id, resultId);
		addPreAction(mCreateCommand);
	}
	return resultId;
}

Id PasteNodeCommand::pasteGraphicalCopy()
{
	Id resultId = mResult;
	if (!mCreateCommand) {
		mCreateCommand = new CreateElementCommand(
				mMVIface->logicalAssistApi()
				, mMVIface->graphicalAssistApi()
				, mMVIface->rootId()
				, mMVIface->rootId()
				, mNodeData.logicalId
				, true
				, mMVIface->graphicalAssistApi()->name(mNodeData.id)
				, mNewPos);
		mCreateCommand->redo();
		resultId = mCreateCommand->result();
		mCopiedIds->insert(mNodeData.id, resultId);
		addPreAction(mCreateCommand);
	}

	NodeElement * const newNode = dynamic_cast<NodeElement *>(
			mScene->mainWindow()->editorManager().graphicalObject(resultId));
	newNode->setAssistApi(mMVIface->graphicalAssistApi(), mMVIface->logicalAssistApi());
	newNode->setController(mScene->mainWindow()->controller());
	newNode->setId(resultId);

	return resultId;
}

void PasteNodeCommand::restoreElement()
{
	Id const logicalId = mMVIface->graphicalAssistApi()->logicalId(mResult);
	mMVIface->graphicalAssistApi()->setProperties(logicalId, mNodeData.logicalProperties);
	mMVIface->graphicalAssistApi()->setProperties(mResult, mNodeData.graphicalProperties);
	if (mCopiedIds->contains(mNodeData.parentId)) {
		mMVIface->graphicalAssistApi()->changeParent(mResult, mCopiedIds->value(mNodeData.parentId), mNewPos);
	}
	NodeElement *element = mScene->getNodeById(mResult);
	if (element) {
		element->updateData();
	}
}

QPointF PasteNodeCommand::getNewPos() const
{
	return mNodeData.pos + (mCopiedIds->contains(mNodeData.parentId) ? QPointF() : mOffset);
}
