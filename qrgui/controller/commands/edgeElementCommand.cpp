#include "edgeElementCommand.h"

using namespace qReal::commands;

EdgeElementCommand::EdgeElementCommand(EditorViewScene const *scene, Id const &id)
	: ElementCommand(scene, id)
{
}

EdgeElementCommand::EdgeElementCommand(EditorView const *view, Id const &id)
	: ElementCommand(view->editorViewScene(), id)
{
}

bool EdgeElementCommand::reinitElement()
{
	ElementCommand::reinitElement();
	mEdge = dynamic_cast<EdgeElement *>(mElement);
	return mEdge != NULL;
}
