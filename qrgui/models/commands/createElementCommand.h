#pragma once

#include "controller/commands/abstractCommand.h"
#include "models/commands/createRemoveCommandImplementation.h"

namespace qReal {
namespace commands {

class CreateElementCommand : public AbstractCommand
{
public:
	CreateElementCommand(
			models::LogicalModelAssistApi &logicalApi
			, models::GraphicalModelAssistApi &graphicalApi
			, models::Exploser const &exploser
			, Id const &logicalParent
			, Id const &graphicalParent
			, Id const &id
			, bool isFromLogicalModel
			, QString const &name
			, QPointF const &position);

	Id result() const;

	/// @todo: Bad method, required only for linkers. Get rid of it.
	/// Modifies command setting new creation position.
	void setNewPosition(QPointF const &position);

protected:
	bool execute() override;
	bool restoreState() override;

private:
	CreateRemoveCommandImplementation mImpl;
	Id mResult;
};

}
}
