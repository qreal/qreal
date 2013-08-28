#pragma once

#include "controller/commands/abstractCommand.h"
#include "toolPluginInterface/usedInterfaces/details/modelsAssistInterface.h"
#include "models/details/exploser.h"

namespace qReal {
namespace commands {

/// Assigns to element with given id given name using given model (logical or graphical)
class RenameCommand : public AbstractCommand
{
public:
	/// This overload gets old name automatically using models api
	RenameCommand(details::ModelsAssistInterface &model
			, Id const &id, QString const &newName
			, models::Exploser * const exploser = NULL);

	/// This overload alows to specify old name manually
	RenameCommand(details::ModelsAssistInterface &model
			, Id const &id, QString const &oldName, QString const &newName
			, models::Exploser * const exploser = NULL);

protected:
	virtual bool execute();
	virtual bool restoreState();

private:
	void initExplosions(models::Exploser * const exploser);

	bool rename(QString const &name);

	details::ModelsAssistInterface &mModel;
	Id const mId;
	QString const mOldName;
	QString const mNewName;
};

}
}
