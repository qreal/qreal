#pragma once

#include "editor/commands/elementCommand.h"

namespace qReal {
namespace commands {

/// A base for all commands using concrete edge element instance on some scene
class EdgeElementCommand : public ElementCommand
{
public:
	EdgeElementCommand(EditorViewScene const *scene, const Id &id);
	EdgeElementCommand(EditorView const *view, const Id &id);
	virtual ~EdgeElementCommand();

protected:
	virtual bool reinitElement();

	EdgeElement *mEdge;
};

}
}
