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

#include "pasteEdgeCommand.h"

#include <qrgui/models/models.h>

#include "editor/commands/reshapeEdgeCommand.h"

using namespace qReal;
using namespace qReal::commands;
using namespace qReal::gui::editor;
using namespace qReal::gui::editor::commands;

PasteEdgeCommand::PasteEdgeCommand(EditorViewScene *scene
		, const EdgeData &data
		, const QPointF &offset
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
		const Id typeId = mEdgeData.id.type();
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

	const Id edgeId = mResult;

	const Id newSrcId = mCopiedIds->value(mEdgeData.srcId);
	const Id newDstId = mCopiedIds->value(mEdgeData.dstId);

	const Id logicalId = mScene->models().graphicalModelAssistApi().logicalId(mCreateCommand->result());
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
