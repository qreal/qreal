#include "insertIntoEdgeCommand.h"

#include "mainWindow/mainWindow.h"

using namespace qReal::commands;

InsertIntoEdgeCommand::InsertIntoEdgeCommand(EditorViewScene &scene
		, models::LogicalModelAssistApi &logicalAssistApi
		, models::GraphicalModelAssistApi &graphicalAssistApi
		, Exploser &exploser
		, Id const &firstElem
		, Id const &lastElem
		, Id const &parent
		, QPointF const &scenePos
		, QPointF const &shift
		, bool isFromLogicalModel
		, CreateElementCommand *createCommand)
	: AbstractCommand()
	, mScene(scene)
	, mLogicalAssistApi(logicalAssistApi)
	, mGraphicalAssistApi(graphicalAssistApi)
	, mExploser(exploser)
	, mFirstId(firstElem)
	, mLastId(lastElem)
	, mParentId(parent)
	, mOldEdge(Id())
	, mPos(scenePos)
	, mShift(shift)
	, mIsLogical(isFromLogicalModel)
	, mCreateFirst(nullptr)
	, mCreateSecond(nullptr)
	, mRemoveOldEdge(nullptr)
	, mCreateCommand(createCommand)
{
	if (mCreateCommand) {
		mCreateCommand->setParent(this);
	}
}

InsertIntoEdgeCommand::~InsertIntoEdgeCommand()
{
	delete mCreateFirst;
	delete mCreateSecond;
	delete mRemoveOldEdge;
}

bool InsertIntoEdgeCommand::execute()
{
	if (mCreateCommand) {
		mCreateCommand->redo();
		mFirstId = mCreateCommand->result();
		mLastId = mCreateCommand->result();
	}

	EdgeElement *edge = mRemoveOldEdge ? mScene.getEdgeById(mOldEdge) : mScene.edgeForInsertion(mPos);
	if (!edge) {
		return true;
	}

	NodeElement *oldSrc = edge->src();
	NodeElement *oldDst = edge->dst();

	if (oldSrc && oldDst && oldSrc->id() != mFirstId && oldDst->id() != mLastId) {
		mParentId = (mParentId == Id::rootId()) ? mScene.rootItemId() : mParentId;
		Id type = edge->id().type();

		initCommand(mCreateFirst, type);
		initCommand(mCreateSecond, type);

		makeLink(mCreateFirst, oldSrc, mScene.getNodeById(mFirstId));
		makeLink(mCreateSecond, mScene.getNodeById(mLastId), oldDst);

		mConfiguration = mGraphicalAssistApi.configuration(edge->id());
		if (!mRemoveOldEdge) {
			mRemoveOldEdge = new RemoveElementCommand(mLogicalAssistApi, mGraphicalAssistApi, mExploser
					, mScene.rootItemId(), mParentId, edge->id(), mIsLogical
					, mGraphicalAssistApi.name(edge->id()), mGraphicalAssistApi.position(edge->id()));
		}
		mRemoveOldEdge->redo();

		mElementShifting.clear();
		mScene.resolveOverlaps(mScene.getNodeById(mLastId), mPos, mShift, mElementShifting);
		mScene.resolveOverlaps(mScene.getNodeById(mFirstId), mPos, mShift, mElementShifting);
	}

	return true;
}

bool InsertIntoEdgeCommand::restoreState()
{
	if (mRemoveOldEdge && mCreateSecond && mCreateFirst) {
		mScene.returnElementsToOldPositions(mElementShifting);

		mRemoveOldEdge->undo();

		mOldEdge = mRemoveOldEdge->elementId();
		EdgeElement *edge = mScene.getEdgeById(mOldEdge);
		edge->setSrc(mScene.getEdgeById(mCreateFirst->result())->src());
		edge->setDst(mScene.getEdgeById(mCreateSecond->result())->dst());
		mScene.reConnectLink(edge);
		mGraphicalAssistApi.setConfiguration(edge->id(), mConfiguration);

		mCreateSecond->undo();
		mCreateFirst->undo();
	}
	if (mCreateCommand) {
		mCreateCommand->undo();
	}
	return true;
}

void InsertIntoEdgeCommand::initCommand(CreateElementCommand *&command, Id const &type)
{
	if (!command) {
		QString const name = mScene.mainWindow()->editorManager().friendlyName(type);
		command = new CreateElementCommand(mLogicalAssistApi, mGraphicalAssistApi, mExploser, mScene.rootItemId()
				, mParentId, Id(type, QUuid::createUuid().toString()), mIsLogical, name, mPos);
	}
}

void InsertIntoEdgeCommand::makeLink(CreateElementCommand *command, NodeElement *src, NodeElement *dst)
{
	command->redo();
	Id newLink = command->result();
	if (src) {
		mGraphicalAssistApi.setFrom(newLink, src->id());
	}

	if (dst) {
		mGraphicalAssistApi.setTo(newLink, dst->id());
	}

	EdgeElement * const edge = mScene.getEdgeById(newLink);
	edge->setSrc(src);
	edge->setDst(dst);
	mScene.reConnectLink(edge);
}
