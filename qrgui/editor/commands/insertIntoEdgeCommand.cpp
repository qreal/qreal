/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "insertIntoEdgeCommand.h"

using namespace qReal;
using namespace qReal::commands;
using namespace qReal::gui::editor::commands;

InsertIntoEdgeCommand::InsertIntoEdgeCommand(EditorViewScene &scene
		, models::LogicalModelAssistApi &logicalAssistApi
		, models::GraphicalModelAssistApi &graphicalAssistApi
		, models::Exploser &exploser
		, const Id &firstElem
		, const Id &lastElem
		, const Id &parent
		, const QPointF &scenePos
		, const QPointF &shift
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

void InsertIntoEdgeCommand::initCommand(CreateElementCommand *&command, const Id &type)
{
	if (!command) {
		const QString name = mLogicalAssistApi.editorManagerInterface().friendlyName(type);
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
