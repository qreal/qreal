#include "repairer.h"

using namespace qReal;

Repairer::Repairer(const EditorManager &editorManager)
	: mEditorManager(editorManager)
{
}

Id Repairer::getCorrectId(const Id target)
{
	readLog(target.diagram());
	return target;
}

void Repairer::readLog(const QString diagram)
{
	if (mLogs.keys().contains(diagram))
		return;
	mLogs.insert(diagram, Message::parseLog("../logs/MetaEditor/" + diagram + "/"));
}
