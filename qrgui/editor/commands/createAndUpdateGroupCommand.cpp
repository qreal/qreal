#include "createAndUpdateGroupCommand.h"

#include "editor/editorViewScene.h"
#include "editor/commands/insertIntoEdgeCommand.h"

using namespace qReal::commands;

CreateAndUpdateGroupCommand::CreateAndUpdateGroupCommand(EditorViewScene &scene
		, models::LogicalModelAssistApi &logicalApi
		, models::GraphicalModelAssistApi &graphicalApi
		, models::Exploser &exploser
		, const Id &logicalParent
		, const Id &graphicalParent
		, const Id &id
		, bool isFromLogicalModel
		, const QPointF &position)
	: CreateGroupCommand(logicalApi, graphicalApi, exploser, logicalParent
			, graphicalParent, id, isFromLogicalModel, position)
	, mScene(scene)
{
	InsertIntoEdgeCommand * const insertCommand = new InsertIntoEdgeCommand(mScene, mLogicalApi, mGraphicalApi
			, mExploser, mNodeCommands[mPattern.inNode()]->result(), mNodeCommands[mPattern.outNode()]->result()
			, mGraphicalParent, mPosition, mPattern.size(), mIsFromLogicalModel);
	insertCommand->setUndoEnabled(false);
	addPostAction(insertCommand);
}

bool CreateAndUpdateGroupCommand::execute()
{
	if (!CreateGroupCommand::execute()) {
		return false;
	}

	for (int i = 0; i < mEdgeCommands.count(); ++i) {
		mScene.reConnectLink(mScene.getEdgeById(mEdgeCommands[i]->result()));
	}

	return true;
}
