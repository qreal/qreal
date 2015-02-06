#pragma once

#include "editor/commands/elementCommand.h"

namespace qReal {
namespace commands {

/// A base for all commands using concrete node element instance on some scene
class NodeElementCommand : public ElementCommand
{
public:
	NodeElementCommand(EditorViewScene const *scene, const Id &id);
	NodeElementCommand(EditorView const *view, const Id &id);
	virtual ~NodeElementCommand();

protected:
	virtual bool reinitElement();

	NodeElement *nodeById(const Id &id);

	NodeElement *mNode;
};

}
}
