#include "nodeElementCommand.h"

using namespace qReal::commands;

NodeElementCommand::NodeElementCommand(const EditorViewScene *scene, const Id &id)
	: ElementCommand(scene, id)
{
}

NodeElementCommand::NodeElementCommand(const EditorView *view, const Id &id)
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
