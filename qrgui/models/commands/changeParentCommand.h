#pragma once

#include "controller/commands/abstractCommand.h"
#include "models/logicalModelAssistApi.h"
#include "models/graphicalModelAssistApi.h"

namespace qReal {
namespace commands {

class QRGUI_MODELS_EXPORT ChangeParentCommand : public AbstractCommand
{
public:
	ChangeParentCommand(
			models::LogicalModelAssistApi &logicalApi
			, models::GraphicalModelAssistApi &graphicalApi
			, bool isLogical, const Id &id, const Id &oldParent
			, const Id newParent, const QPointF &oldPosition
			, const QPointF &newPosition);

protected:
	bool execute();
	bool restoreState();

private:
	void changeParent(const Id &parent, const QPointF &position);

	models::LogicalModelAssistApi &mLogicalApi;
	models::GraphicalModelAssistApi &mGraphicalApi;
	bool mIsLogical;
	Id mId;
	Id mOldParent;
	Id mNewParent;
	QPointF mOldPosition;
	QPointF mNewPosition;
};

}
}
