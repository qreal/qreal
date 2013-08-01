#pragma once

#include "abstractCommand.h"
#include "trackingEntity.h"
#include "../../models/logicalModelAssistApi.h"
#include "../../mainwindow/palette/paletteTreeWidget.h"
#include "../../mainwindow/palette/draggableElement.h"

namespace qReal {
namespace commands {

/// This command triggeres palette item editing process for given explosion instance
class RenameExplosionCommand : public AbstractCommand
{
	Q_OBJECT

public:
	RenameExplosionCommand(models::LogicalModelAssistApi * const logicalApi
		, models::GraphicalModelAssistApi const * const graphicalApi
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

	models::LogicalModelAssistApi *mLogicalApi;
	models::GraphicalModelAssistApi const *mGraphicalApi;
	Id mTarget;
	QString mOldName;
	QString mNewName;
};

}
}
