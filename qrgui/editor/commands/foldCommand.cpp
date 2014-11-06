#include "foldCommand.h"

using namespace qReal::commands;

FoldCommand::FoldCommand(NodeElement const *element)
	: NodeElementCommand(dynamic_cast<EditorViewScene *>(element->scene()), element->id())
{
}

bool FoldCommand::execute()
{
	NodeElementCommand::execute();
	changeFoldState();
	return true;
}

bool FoldCommand::restoreState()
{
	NodeElementCommand::restoreState();
	changeFoldState();
	return true;
}

void FoldCommand::changeFoldState()
{
	if (mNode) {
		mNode->changeFoldState();
	}
}
