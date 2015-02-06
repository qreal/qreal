#pragma once

#include "controller/commands/abstractCommand.h"
#include "models/logicalModelAssistApi.h"
#include "models/graphicalModelAssistApi.h"

namespace qReal {
namespace commands {

/// Adds or removes explosion between specified elements
class QRGUI_MODELS_EXPORT ExplosionCommand : public AbstractCommand
{
public:
	/// Creates new instance of a command that creates or removes explosions.
	/// If graphical assist api parameter specified then it is allowed to
	/// pass graphical ids as source or target parameters.
	/// Otherwise both source and target must be logical ids.
	/// @param mustAdd Specifies if this command should create or remove explosions.
	ExplosionCommand(models::LogicalModelAssistApi &logicalApi
			, models::GraphicalModelAssistApi const * const graphicalApi
			, const Id &source, const Id &target, bool mustAdd);

	virtual ~ExplosionCommand();

protected:
	virtual bool execute();
	virtual bool restoreState();

private:
	bool processExplosion(bool add);

	models::LogicalModelAssistApi &mLogicalApi;
	models::GraphicalModelAssistApi const * const mGraphicalApi;  // Doesn't take ownership.
	Id mSource;
	Id mTarget;
	const bool mMustAdd;
};

}
}
