#include "repairer.h"

using namespace qReal;

Repairer::Repairer(const EditorManager &editorManager)
	: mEditorManager(editorManager)
{

}

Id Repairer::getCorrectId(const Id target)
{
	qDebug() << "Repairer::getCorrectId()";
	qDebug() << target.toString();

	readLog(target.editorId());

	return target;
}

void Repairer::readLog(const Id editor)
{
	qDebug() << "readLog()";
	qDebug() << editor.toString();

	if (!mLogs.keys().contains(editor)) {
		QList<Message> *log;
		mLogs.insert(editor, log);
		QString path = "../logs/MetaEditor/";	//not sure
//		log->append();
	}
}
