#pragma once

#include "editor/commands/elementCommand.h"

namespace qReal {
namespace commands {

class UpdateElementCommand : public ElementCommand
{
public:
	UpdateElementCommand(EditorViewScene const *scene, Id const &id);
	UpdateElementCommand(EditorView const *view, Id const &id);
	virtual ~UpdateElementCommand();

	virtual bool equals(AbstractCommand const &other) const;

protected:
	virtual bool execute();
	virtual bool restoreState();

private:
	bool update();
};

}
}
