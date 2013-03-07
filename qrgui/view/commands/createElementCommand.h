#pragma once

#include "../../controller/abstractCommand.h"
#include "createRemoveCommandImplementation.h"

namespace qReal
{
namespace commands
{

class CreateElementCommand : public AbstractCommand
{
public:
	CreateElementCommand(models::GraphicalModelAssistApi *api
			, Id const &parent
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
