#pragma once

#include "editor/commands/elementCommand.h"

namespace qReal {
namespace commands {

/// A base for all commands using concrete edge element instance on some scene
class EdgeElementCommand : public ElementCommand
{
public:
	EdgeElementCommand(EditorViewScene const *scene, Id const &id);
	EdgeElementCommand(EditorView const *view, Id const &id);
	virtual ~EdgeElementCommand();

protected:
	virtual bool reinitElement();

	EdgeElement *mEdge;
};

}
}
