#include "repairer.h"

#include "../mainwindow/mainwindow.h"

using namespace qReal;

Repairer::Repairer(qrRepo::RepoApi &api, EditorManager const &editorManager)
	:
	mApi(api),
	mEditorManager(editorManager)
{
	inProgress = false;
	lastOpenedSave = "";
}

MainWindow* Repairer::getMainWindow() const
{
	return mEditorManager.getMainWindow();
}

void Repairer::rememberSave(QString savePath)
{
	lastOpenedSave = savePath;
}

bool Repairer::checkId(Id target) const
{
	return (mEditorManager.elements(target.diagramId()).contains(target.type()));
}

bool Repairer::checkIds(Id const target)
{
	if ((target != Id::getRootId()) && (!checkId(target)))
		return false;

	foreach(Id child, mApi.children(target)) {
		if (!checkIds(child))
			return false;
	}

	return true;
}

void Repairer::repair()
{
	if (inProgress)
		return;
	inProgress = true;
	mRepairerDialog = new RepairerDialog(this, lastOpenedSave);
	mRepairerDialog->show();
}

void Repairer::repairElements(const Id target)
{
	static int counter = 0;
	counter++;

	foreach(Id child, mApi.children(target)) {
		if (child.idSize() == 0)
			continue;
		if (!checkId(child)) {
			Id newId = correctId(child);
			replace(child, newId);
			repairElements(newId);
			continue;
		}
		repairElements(child);
	}

	counter--;
	if (counter == 0)
		emit workFinished();
	inProgress = false;
}

Id Repairer::correctId(const Id target)
{
	QLinkedList<Message> log = Message::parseLog("../logs/MetaEditor/" + target.diagram() + '/');
	Id curr = Id::loadFromString(target.toString().remove('/' + target.id()));
	foreach(Message message, log)
		if ((message.valid()) && (message.prevValue().type() == QVariant::String) &&
			(message.newValue().type() == QVariant::String) && (message.prevValue().toString() == curr.element()))
				curr = Id::loadFromString(target.diagramId().toString() + '/' + message.newValue().toString());

	if (mEditorManager.elements(target.diagramId()).contains(curr))
		return Id::loadFromString(curr.toString() + '/' + target.id());
	else {
		qDebug() << "Repairer::correctId() error | There is no final element in editor (incorrect log?).";
		return Id();
	}
}

void Repairer::changeType(Id const target, QString const &diagramName,
		QString const prevType, QString const newType) const
{
	IdList childs = mApi.children(target);
	if (childs.empty())
		return;
	foreach(Id child, childs) {
		changeType(child, diagramName, prevType, newType);
		if ((child.diagram() == diagramName) && (child.element() == prevType))
			replace(child, Id(child.editor(),child.diagram(),newType,child.id()));
	}
}

void Repairer::replace(const Id from, const Id to) const
{
	mApi.replace(from, to);
}

void Repairer::patchSave()
{
	mPatchSaveDialog = new PatchSaveDialog(lastOpenedSave,"",this);
	mPatchSaveDialog->show();
//	remember: release memory
}

void Repairer::patchSave(QString savePath, QString patchPath)
{
	Id rootId = qReal::Id::getRootId();

	mApi.open(savePath);

	int count = 0;
	QLinkedList<Message> log = Message::parseLog(patchPath);
	foreach(Message message, log) {
		if (!message.valid())
			continue;
		Id target = message.target();
		switch (message.performed()) {
			case qReal::actAddElement: {
				Id parent;
				foreach(Id child, mApi.children(rootId))
					if (child.diagramId() == target.diagramId())
						parent = child;
				if (mApi.exist(parent)) {
					count++;
					mApi.addChild(parent, target);
					QString name = target.element();
					name += ' ' + QString::number(count);
					mApi.setProperty(target, "name", name);
					mApi.setProperty(target, "position", QPointF());
					mApi.setProperty(target, "configuration", QPolygon());
					if (mEditorManager.getEditorInterface(
							target.editor())->isNodeOrEdge(target.element()) == -1) {
						mApi.setProperty(target, "to", ROOT_ID.toVariant());
						mApi.setProperty(target, "from", ROOT_ID.toVariant());
						mApi.setProperty(target, "toPort", 0.0);
						mApi.setProperty(target, "fromPort", 0.0);
					}
				}
				else
					qDebug() <<
					"Repairer::patchSave() error | It isn't obvious where element must be placed.";
				break;
			}
			case qReal::actRemoveElement: {
				if (mApi.exist(target)) {
					foreach(Id parent, mApi.parents(target))
						mApi.removeChild(parent, target);
				}
				else
					qDebug() <<
					"Repairer::patchSave() error | There is no element to remove.";
				break;
			}
			case qReal::actSetData: {
				mApi.setProperty(target, message.details(), message.newValue());
				break;
			}
			case qReal::actChangeType: {
				changeType(Id::getRootId(), message.details(),
					message.prevValue().toString(), message.newValue().toString());
				break;
			}
			default:
				qDebug() << "Unused operation.";
		}
	}

	emit workFinished();
	inProgress = false;
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

bool Repairer::isBusy()
{
	return inProgress;
}
