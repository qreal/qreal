#include "nodeElementCommand.h"

using namespace qReal::commands;

NodeElementCommand::NodeElementCommand(EditorViewScene const *scene, const Id &id)
	: ElementCommand(scene, id)
{
}

NodeElementCommand::NodeElementCommand(EditorView const *view, const Id &id)
	: ElementCommand(&view->editorViewScene(), id)
{
}

NodeElementCommand::~NodeElementCommand()
{
}

bool NodeElementCommand::reinitElement()
{
	if (!ElementCommand::reinitElement()) {
		return false;
	}
	mNode = dynamic_cast<NodeElement *>(mElement);
	return mNode != nullptr;
}

NodeElement *NodeElementCommand::nodeById(const Id &id)
{
	return dynamic_cast<NodeElement *>(elementById(id));
}
