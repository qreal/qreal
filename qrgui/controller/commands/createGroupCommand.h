#pragma once

#include "abstractCommand.h"
#include "createElementCommand.h"
#include "../../view/editorViewScene.h"

namespace qReal
{
namespace commands
{

class CreateGroupCommand : public AbstractCommand
{
public:
	CreateGroupCommand(EditorViewScene * const scene
			, models::LogicalModelAssistApi * const logicalApi
			, models::GraphicalModelAssistApi * const graphicalApi
			, Id const &logicalParent
			, Id const &graphicalParent
			, Id const &id
			, bool isFromLogicalModel
			, QPointF const &position);

protected:
	virtual bool execute();
	virtual bool restoreState();

private:
	EditorViewScene * const mScene;  // Doesn`t take ownership
	models::LogicalModelAssistApi * const mLogicalApi; // Doesn`t take ownership
	models::GraphicalModelAssistApi * const mGraphicalApi; // Doesn`t take ownership
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
