#pragma once

#include "abstractCommand.h"
#include "../../models/logicalModelAssistApi.h"
#include "../../models/graphicalModelAssistApi.h"

namespace qReal
{
namespace commands
{

/// Adds or removes explosion between specified elements
class ExplosionCommand : public AbstractCommand
{
public:
	/// Creates new instance of explosion command using either logical or graphical
	/// is as target autodetecting corresponding situations
	ExplosionCommand(models::LogicalModelAssistApi * const logicalApi
			, models::GraphicalModelAssistApi const * const graphicalApi
			, Id const &source, Id const &target, bool mustAdd);
	virtual ~ExplosionCommand();

protected:
	virtual bool execute();
	virtual bool restoreState();

private:
	bool processExplosion(bool add);

	models::LogicalModelAssistApi *mLogicalApi;
	models::GraphicalModelAssistApi const *mGraphicalApi;
	Id mSource;
	Id mTarget;
	bool const mMustAdd;
};

}
}
