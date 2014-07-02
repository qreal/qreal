#pragma once

#include "controller/commands/abstractCommand.h"
#include "controller/commands/createRemoveCommandImplementation.h"
#include "umllib/element.h"

namespace qReal {
namespace commands {

class RemoveElementCommand : public AbstractCommand
{
public:
	RemoveElementCommand(
			models::LogicalModelAssistApi &logicalApi
			, models::GraphicalModelAssistApi &graphicalApi
			, Exploser &exploser
			, Id const &logicalParent
			, Id const &graphicalParent
			, Id const &id
			, bool isFromLogicalModel
			, QString const &name
			, QPointF const &position);

	RemoveElementCommand(
			models::LogicalModelAssistApi &logicalApi
			, models::GraphicalModelAssistApi &graphicalApi
			, Exploser &exploser
			, const Id &logicalParent
			, const Id &graphicalParent
			, bool isFromLogicalModel
			, Element *element);

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
