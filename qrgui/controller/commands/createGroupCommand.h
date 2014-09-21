#pragma once

#include "models/commands/createElementCommand.h"
#include "editor/editorViewScene.h"

namespace qReal {
namespace commands {

class CreateGroupCommand : public AbstractCommand
{
public:
	CreateGroupCommand(EditorViewScene * const scene
			, models::LogicalModelAssistApi &logicalApi
			, models::GraphicalModelAssistApi &graphicalApi
			, Exploser &exploser
			, Id const &logicalParent
			, Id const &graphicalParent
			, Id const &id
			, bool isFromLogicalModel
			, QPointF const &position);

	Id rootId() const;

protected:
	virtual bool execute();
	virtual bool restoreState();

private:

	EditorViewScene *mScene;
	models::LogicalModelAssistApi &mLogicalApi;
	models::GraphicalModelAssistApi &mGraphicalApi;
	Exploser &mExploser;
	Id const mLogicalParent;
	Id const mGraphicalParent;
	Id const mId;
	bool const mIsFromLogicalModel;
	QPointF const mPosition;
	Pattern const mPattern;
	QMap<QString, CreateElementCommand *> mNodeCommands;
	QList<CreateElementCommand *> mEdgeCommands;
	Id mRootId;
};

}
}
