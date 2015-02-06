#pragma once

#include "controller/commands/abstractCommand.h"
#include "models/commands/createRemoveCommandImplementation.h"

namespace qReal {
namespace commands {

class QRGUI_MODELS_EXPORT CreateElementCommand : public AbstractCommand
{
public:
	CreateElementCommand(
			models::LogicalModelAssistApi &logicalApi
			, models::GraphicalModelAssistApi &graphicalApi
			, models::Exploser const &exploser
			, const Id &logicalParent
			, const Id &graphicalParent
			, const Id &id
			, bool isFromLogicalModel
			, const QString &name
			, const QPointF &position);

	Id result() const;

	/// @todo: Bad method, required only for linkers. Get rid of it.
	/// Modifies command setting new creation position.
	void setNewPosition(const QPointF &position);

protected:
	bool execute() override;
	bool restoreState() override;

private:
	CreateRemoveCommandImplementation mImpl;
	Id mResult;
};

}
}
