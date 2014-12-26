#pragma once

#include "controller/commands/doNothingCommand.h"
#include "models/modelsDeclSpec.h"

namespace qReal {

namespace models {
class Models;
class Exploser;
class LogicalModelAssistApi;
class GraphicalModelAssistApi;
}

namespace commands {

/// Accepts a list of ids and removes them from model.
class QRGUI_MODELS_EXPORT MultipleRemoveCommand : public DoNothingCommand
{
public:
	explicit MultipleRemoveCommand(models::Models const &models);

	/// Initializes subcommands that will remove and restore the given elements.
	/// This paremeter can`t be passed into the constructor because it calls some
	/// virtual methods. The list will be cleared in result.
	void setItemsToDelete(IdList &itemsToDelete);

	/// Produces and instance of command that removes logical element and all of its graphical elements.
	commands::AbstractCommand *logicalDeleteCommand(QModelIndex const &index);

	/// Produces and instance of command that removes the given graphical element.
	commands::AbstractCommand *graphicalDeleteCommand(QModelIndex const &index);

protected:
	void addEdgesToBeDeleted(IdList &itemsToDelete);

	virtual commands::AbstractCommand *logicalDeleteCommand(Id const &index);
	virtual commands::AbstractCommand *graphicalDeleteCommand(Id const &index);
	void appendExplosionsCommands(commands::AbstractCommand *parentCommand, Id const &logicalId);

	models::LogicalModelAssistApi &mLogicalApi;
	models::GraphicalModelAssistApi &mGraphicalApi;
	models::Exploser &mExploser;
};

}
}
