#pragma once

#include "editor/commands/elementCommand.h"

namespace qReal {
namespace commands {

/// A base for all commands using concrete node element instance on some scene
class NodeElementCommand : public ElementCommand
{
public:
	NodeElementCommand(EditorViewScene const *scene, Id const &id);
	NodeElementCommand(EditorView const *view, Id const &id);
	virtual ~NodeElementCommand();

protected:
	virtual bool reinitElement();

	NodeElement *nodeById(Id const &id);

	NodeElement *mNode;
};

}
}
