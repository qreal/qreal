#include "nodeElementCommand.h"

using namespace qReal::commands;

NodeElementCommand::NodeElementCommand(EditorViewScene const *scene, Id const &id)
	: ElementCommand(scene, id)
{
}

NodeElementCommand::NodeElementCommand(EditorView const *view, Id const &id)
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
	return mNode != NULL;
}

NodeElement *NodeElementCommand::nodeById(Id const &id)
{
	return dynamic_cast<NodeElement *>(elementById(id));
}
