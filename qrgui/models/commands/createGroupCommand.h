#pragma once

#include "models/commands/createElementCommand.h"
#include "plugins/pluginManager/pattern.h"

namespace qReal {
namespace commands {

class CreateGroupCommand : public AbstractCommand
{
public:
	CreateGroupCommand(models::LogicalModelAssistApi &logicalApi
			, models::GraphicalModelAssistApi &graphicalApi
			, models::Exploser &exploser
			, Id const &logicalParent
			, Id const &graphicalParent
			, Id const &id
			, bool isFromLogicalModel
			, QPointF const &position);

	Id rootId() const;

protected:
	bool execute() override;
	bool restoreState() override;

	models::LogicalModelAssistApi &mLogicalApi;
	models::GraphicalModelAssistApi &mGraphicalApi;
	models::Exploser &mExploser;
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
