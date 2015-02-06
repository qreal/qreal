#pragma once

#include "controller/commands/abstractCommand.h"
#include "models/commands/createRemoveCommandImplementation.h"

namespace qReal {
namespace commands {

class QRGUI_MODELS_EXPORT RemoveElementCommand : public AbstractCommand
{
public:
	RemoveElementCommand(
			models::LogicalModelAssistApi &logicalApi
			, models::GraphicalModelAssistApi &graphicalApi
			, models::Exploser const &exploser
			, const Id &logicalParent
			, const Id &graphicalParent
			, const Id &id
			, bool isFromLogicalModel
			, const QString &name
			, const QPointF &position);

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
