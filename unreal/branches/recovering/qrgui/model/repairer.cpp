#include "repairer.h"

#include "../mainwindow/mainwindow.h"

using namespace qReal;

Repairer::Repairer(qrRepo::RepoApi &api, const EditorManager &editorManager)
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
			mApi.replace(child, newId);
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
	foreach(Message msg, log)
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

void Repairer::replace(const Id from, const Id to) const
{
	//trash for analysis
/**
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
**/
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
	foreach(Message msg, log) {
		Id target = msg.target();
		switch (msg.performed()) {
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
				mApi.setProperty(target, msg.details(), msg.newValue());
				break;
			}
			case qReal::ptchReplaceElement: {
				if (msg.details() == qReal::msgAllElements) {
					Id parent;
					Id diagram = msg.target();
					if (diagram.diagramId() != diagram) {
						qDebug() <<
						"Repairer::patchSave() error | Not Implemented yet.";
						break;	//I hope it is correct (it must quit from case).
					}

					foreach(Id child, mApi.children(rootId))
						if (child.diagramId() == diagram)
							parent = child;
					foreach(Id element, mEditorManager.elementsOnDiagram(parent)) {
						if (element.element() == msg.prevValue().toString())
							replace(element, Id(element.editor(),element.diagram(),
								msg.newValue().toString(),element.id()));
					}
				}
				else
					qDebug() <<
					"Repairer::patchSave() error | Not Implemented yet.";
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
