#pragma once

#include "controller/commands/abstractCommand.h"
#include "controller/commands/createElementCommand.h"
#include "view/editorViewScene.h"

namespace qReal {
namespace commands {

class CreateGroupCommand : public AbstractCommand
{
public:
	CreateGroupCommand(EditorViewScene &scene
			, models::LogicalModelAssistApi &logicalApi
			, models::GraphicalModelAssistApi &graphicalApi
			, Id const &logicalParent
			, Id const &graphicalParent
			, Id const &id
			, bool isFromLogicalModel
			, QPointF const &position);

protected:
	virtual bool execute();
	virtual bool restoreState();

private:
	EditorViewScene &mScene;
	models::LogicalModelAssistApi &mLogicalApi;
	models::GraphicalModelAssistApi &mGraphicalApi;
	Id const mLogicalParent;
	Id const mGraphicalParent;
	Id const mId;
	bool const mIsFromLogicalModel;
	QPointF const mPosition;
	Pattern const mPattern;
	QList<CreateElementCommand *> mNodeCommands;
	QList<CreateElementCommand *> mEdgeCommands;
};

}
}
