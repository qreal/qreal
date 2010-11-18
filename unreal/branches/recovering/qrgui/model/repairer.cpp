#include "repairer.h"

#include "../mainwindow/mainwindow.h"

using namespace qReal;

Repairer::Repairer(qrRepo::RepoApi &api, const EditorManager &editorManager)
	:
	mApi(api),
	mEditorManager(editorManager)
{
	inProgress = false;
}

bool Repairer::isBusy()
{
	return inProgress;
}

void Repairer::patchSave()
{
	mPatchSaveDialog = new PatchSaveDialog("","",this);
	mPatchSaveDialog->show();
//	remember: release memory
}

void Repairer::patchSave(QString savePath, QString patchPath)
{
//	todo:
//	пропатчить редактор с помощью специального лога
//	просто выполнить последовательно всё, что в логе
//	следует избегать ситуаций с двумя разными ветками
}

void Repairer::repair()
{
	inProgress = true;
	mRepairerDialog = new RepairerDialog(this);
	mRepairerDialog->show();
}

void Repairer::repairElements(const Id target)
{
	foreach(Id child, mApi.children(target)) {
		if (child.idSize() == 0)
			continue;
		if (!isCorrect(child)) {
			Id newId = correctId(child);
			mApi.replace(child, newId);
			repairElements(newId);
			continue;
		}
		repairElements(child);
	}

	emit workFinished();
	inProgress = false;
}

MainWindow* Repairer::getMainWindow() const
{
	return mEditorManager.getMainWindow();
}

void Repairer::readLog(const QString diagram)
{
	if (mLogs.keys().contains(diagram))
		return;
	mLogs.insert(diagram, Message::parseLog("../logs/MetaEditor/" + diagram + "/"));
}

bool Repairer::isCorrect(Id target) const
{
	return (mEditorManager.elements(target.diagramId()).contains(target.type()));
}

bool Repairer::checkIds(Id const target)
{
	if ((target != Id::getRootId()) && (!isCorrect(target)))
		return false;

	foreach(Id child, mApi.children(target)) {
		if (!checkIds(child))
			return false;
	}

	return true;
}

Id Repairer::correctId(const Id target)
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
		qDebug() << "Repairer::correctId() error | There is no final element in editor (incorrect log?).";
		return Id();
	}
}

void Repairer::releaseRepairerDialog()
{
	inProgress = false;
	delete mRepairerDialog;
}

void Repairer::releasePatchSaveDialog()
{
	delete mPatchSaveDialog;
}
