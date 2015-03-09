#pragma once

#include "editor/commands/elementCommand.h"

namespace qReal {
namespace commands {

class UpdateElementCommand : public ElementCommand
{
public:
	UpdateElementCommand(const EditorViewScene *scene, const Id &id);
	UpdateElementCommand(const EditorView *view, const Id &id);
	virtual ~UpdateElementCommand();

	virtual bool equals(const AbstractCommand &other) const;

protected:
	virtual bool execute();
	virtual bool restoreState();

private:
	bool update();
};

}
}
