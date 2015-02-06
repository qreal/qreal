#include "edgeElementCommand.h"

using namespace qReal::commands;

EdgeElementCommand::EdgeElementCommand(EditorViewScene const *scene, const Id &id)
	: ElementCommand(scene, id)
{
}

EdgeElementCommand::EdgeElementCommand(EditorView const *view, const Id &id)
	: ElementCommand(&view->editorViewScene(), id)
{
}

EdgeElementCommand::~EdgeElementCommand()
{
}

bool EdgeElementCommand::reinitElement()
{
	if (!ElementCommand::reinitElement()) {
		return false;
	}
	mEdge = dynamic_cast<EdgeElement *>(mElement);
	return mEdge != nullptr;
}
