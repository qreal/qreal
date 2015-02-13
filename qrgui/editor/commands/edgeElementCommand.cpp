#include "edgeElementCommand.h"

using namespace qReal::commands;

EdgeElementCommand::EdgeElementCommand(const EditorViewScene *scene, const Id &id)
	: ElementCommand(scene, id)
{
}

EdgeElementCommand::EdgeElementCommand(const EditorView *view, const Id &id)
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
