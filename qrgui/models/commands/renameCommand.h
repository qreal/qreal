#pragma once

#include "controller/commands/abstractCommand.h"
#include "models/exploser.h"
#include "plugins/toolPluginInterface/usedInterfaces/details/modelsAssistInterface.h"

namespace qReal {
namespace commands {

/// Assigns to element with given id given name using given model (logical or graphical)
class QRGUI_MODELS_EXPORT RenameCommand : public AbstractCommand
{
public:
	/// This overload gets old name automatically using models api
	RenameCommand(details::ModelsAssistInterface &model
			, const Id &id, const QString &newName
			, const models::Exploser * const exploser = nullptr);

	/// This overload alows to specify old name manually
	RenameCommand(details::ModelsAssistInterface &model
			, const Id &id, const QString &oldName, const QString &newName
			, const models::Exploser * const exploser = nullptr);

protected:
	virtual bool execute();
	virtual bool restoreState();

private:
	void initExplosions(const models::Exploser * const exploser);

	bool rename(const QString &name);

	details::ModelsAssistInterface &mModel;
	const Id mId;
	const QString mOldName;
	const QString mNewName;
};

}
}
