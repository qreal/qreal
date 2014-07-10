#pragma once

#include "controller/commands/abstractCommand.h"
#include "controller/commands/createRemoveCommandImplementation.h"

namespace qReal {
namespace commands {

class CreateElementCommand : public AbstractCommand
{
public:
	CreateElementCommand(
			models::LogicalModelAssistApi &logicalApi
			, models::GraphicalModelAssistApi &graphicalApi
			, Exploser &exploser
			, Id const &logicalParent
			, Id const &graphicalParent
			, Id const &id
			, bool isFromLogicalModel
			, QString const &name
			, QPointF const &position);

	Id result() const;

protected:
	virtual bool execute();
	virtual bool restoreState();

private:
	CreateRemoveCommandImplementation mImpl;
	Id mResult;
};

}
}
