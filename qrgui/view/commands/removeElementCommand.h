#pragma once

#include "../../controller/abstractCommand.h"
#include "createRemoveCommandImplementation.h"

namespace qReal
{
namespace commands
{

class RemoveElementCommand : public AbstractCommand
{
public:
	RemoveElementCommand(
			models::LogicalModelAssistApi *logicalApi
			, models::GraphicalModelAssistApi *graphicalApi
			, Id const &parent
			, Id const &id
			, bool isFromLogicalModel
			, QString const &name
			, QPointF const &position);

protected:
	virtual bool execute();
	virtual bool restoreState();

private:
	CreateRemoveCommandImplementation mImpl;
};

}
}
