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

EdgeElementCommand::~EdgeElementCommand()
{
}

bool EdgeElementCommand::reinitElement()
{
	if (!ElementCommand::reinitElement()) {
		return false;
	}
	mEdge = dynamic_cast<EdgeElement *>(mElement);
	return mEdge != NULL;
}
