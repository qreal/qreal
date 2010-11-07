#include "repairer.h"

using namespace qReal;

Repairer::Repairer(const EditorManager &editorManager)
	: mEditorManager(editorManager)
{
}

Id Repairer::getCorrectId(const Id target)
{
	readLog(target.diagram());

	Id curr = Id::loadFromString(target.toString().remove('/' + target.id()));
	foreach(Message msg, mLogs.value(target.diagram()))
		if ((msg.prevValue().type() == QVariant::String) && (msg.newValue().type() == QVariant::String)
			&& (msg.prevValue().toString() == curr.element()))
				curr = Id::loadFromString(target.diagramId().toString() + '/' + msg.newValue().toString());

	if (mEditorManager.elements(target.diagramId()).contains(curr))
		return Id::loadFromString(curr.toString() + '/' + target.id());
	else {
		qDebug() << "Repairer::getCorrectId() error | There is no final element in editor (incorrect log?).";
		return Id();
	}
}

void Repairer::readLog(const QString diagram)
{
	if (mLogs.keys().contains(diagram))
		return;
	mLogs.insert(diagram, Message::parseLog("../logs/MetaEditor/" + diagram + "/"));
}
