#pragma once

#include "models/commands/createElementCommand.h"
#include "plugins/pluginManager/pattern.h"

namespace qReal {
namespace commands {

class QRGUI_MODELS_EXPORT CreateGroupCommand : public AbstractCommand
{
public:
	CreateGroupCommand(models::LogicalModelAssistApi &logicalApi
			, models::GraphicalModelAssistApi &graphicalApi
			, models::Exploser &exploser
			, const Id &logicalParent
			, const Id &graphicalParent
			, const Id &id
			, bool isFromLogicalModel
			, const QPointF &position);

	Id rootId() const;

protected:
	bool execute() override;
	bool restoreState() override;

	models::LogicalModelAssistApi &mLogicalApi;
	models::GraphicalModelAssistApi &mGraphicalApi;
	models::Exploser &mExploser;
	const Id mLogicalParent;
	const Id mGraphicalParent;
	const Id mId;
	const bool mIsFromLogicalModel;
	const QPointF mPosition;
	Pattern const mPattern;
	QMap<QString, CreateElementCommand *> mNodeCommands;
	QList<CreateElementCommand *> mEdgeCommands;
	Id mRootId;
};

}
}
