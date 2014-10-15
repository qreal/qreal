#pragma once

#include "controller/commands/abstractCommand.h"
#include "models/commands/createRemoveCommandImplementation.h"

namespace qReal {
namespace commands {

class RemoveElementCommand : public AbstractCommand
{
public:
	RemoveElementCommand(
			models::LogicalModelAssistApi &logicalApi
			, models::GraphicalModelAssistApi &graphicalApi
			, models::Exploser const &exploser
			, Id const &logicalParent
			, Id const &graphicalParent
			, Id const &id
			, bool isFromLogicalModel
			, QString const &name
			, QPointF const &position);

	virtual bool equals(AbstractCommand const &other) const;

	Id elementId() const;

protected:
	virtual bool execute();
	virtual bool restoreState();

private:
	CreateRemoveCommandImplementation mImpl;
};

}
}
