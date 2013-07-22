#pragma once

#include "abstractCommand.h"
#include "../toolPluginInterface/usedInterfaces/details/modelsAssistInterface.h"

namespace qReal
{
namespace commands
{

/// Assigns to element with given id given name using given model (logical or graphical)
class RenameCommand : public AbstractCommand
{
public:
	RenameCommand(details::ModelsAssistInterface * const model
			, Id const &id, QString const &newName);

protected:
	virtual bool execute();
	virtual bool restoreState();

private:
	bool rename(QString const &name);

	details::ModelsAssistInterface *mModel;
	Id const mId;
	QString const mOldName;
	QString const mNewName;
};

}
}
