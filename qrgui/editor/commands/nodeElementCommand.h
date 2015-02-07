#pragma once

#include "editor/commands/elementCommand.h"

namespace qReal {
namespace commands {

/// A base for all commands using concrete node element instance on some scene
class NodeElementCommand : public ElementCommand
{
public:
	NodeElementCommand(const EditorViewScene *scene, const Id &id);
	NodeElementCommand(const EditorView *view, const Id &id);
	virtual ~NodeElementCommand();

protected:
	virtual bool reinitElement();

	NodeElement *nodeById(const Id &id);

	NodeElement *mNode;
};

}
}
