#pragma once

#include "abstractCommand.h"
#include "../../models/logicalModelAssistApi.h"
#include "../../models/graphicalModelAssistApi.h"

namespace qReal {
namespace commands {

class ChangeParentCommand : public AbstractCommand
{
public:
	ChangeParentCommand(
			models::LogicalModelAssistApi * const logicalApi
			, models::GraphicalModelAssistApi * const graphicalApi
			, bool isLogical, Id const &id, Id const &oldParent
			, Id const newParent, QPointF const &oldPosition
			, QPointF const &newPosition);

protected:
	bool execute();
	bool restoreState();

private:
	void changeParent(Id const &parent, QPointF const &position);

	models::LogicalModelAssistApi *mLogicalApi;
	models::GraphicalModelAssistApi *mGraphicalApi;
	bool mIsLogical;
	Id mId;
	Id mOldParent;
	Id mNewParent;
	QPointF mOldPosition;
	QPointF mNewPosition;
};

}
}
