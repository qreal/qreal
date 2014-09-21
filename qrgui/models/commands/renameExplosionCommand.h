#pragma once

#include "controller/commands/abstractCommand.h"
#include "controller/commands/trackingEntity.h"
#include "models/exploser.h"

namespace qReal {
namespace commands {

/// This command triggeres palette item editing process for given explosion instance
class RenameExplosionCommand : public AbstractCommand
{
	Q_OBJECT

public:
	RenameExplosionCommand(models::LogicalModelAssistApi &logicalApi
		, models::GraphicalModelAssistApi const * const graphicalApi
		, Exploser &exploser
		, Id const &target);

	virtual ~RenameExplosionCommand();

protected:
	virtual bool execute();
	virtual bool restoreState();

private:
	void ensureLogicalId();

	void promptUserToEnterNewName();

	QTreeWidgetItem *userGroup() const;
	QTreeWidgetItem *paletteItem() const;

	models::LogicalModelAssistApi &mLogicalApi;
	models::GraphicalModelAssistApi const *mGraphicalApi;
	Exploser &mExploser;
	Id mTarget;
	QString mOldName;
	QString mNewName;
};

}
}
