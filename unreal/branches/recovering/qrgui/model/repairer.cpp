#include "repairer.h"

using namespace qReal;

Repairer::Repairer(qrRepo::RepoApi &api, const EditorManager &editorManager)
	:
	mApi(api),
	mEditorManager(editorManager)
{

}

void Repairer::patchSave()
{
	qDebug() << "patchSave()";
	//пропатчить редактор с помощью специального лога
	//просто выполнить последовательно всё, что в логе
	//следует избегать ситуаций с двумя разными ветками
}

void Repairer::patchEditor()
{
	qDebug() << "patchEditor()";
	//пропатчить редактор с помощью специального лога
	//просто выполнить последовательно всё, что в логе
	//следует избегать ситуаций с двумя разными ветками
}

void Repairer::repair()
{
	qDebug() << "Repairing...";
	//предложить Автопочинку/ПропатчитьСейв/ПропатчитьРедактор
	repairElements(Id::getRootId());
	qDebug() << "Finished.";
}

void Repairer::repairElements(const Id target)
{
	foreach(Id child, mApi.children(target)) {
		//надо это оптимизировать, чтобы не проверять по 2 раза элементы
		if (!isCorrect(child)) {
			Id newId = correctId(child);
			mApi.replace(child, newId);
			repairElements(newId);
			continue;
		}
		repairElements(child);
	}
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

bool Repairer::process(Id const target)
{
	//критерии проверки надо будет изменить и вынести в отдельные методы
	if ((target != Id::getRootId()) && (!isCorrect(target))) {
		repairElements();
		return false;
	}
	foreach(Id child, mApi.children(target)) {
		if (!process(child)) {
			repairElements();
			return false;
		}
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
		qDebug() << "Repairer::getCorrectId() error | There is no final element in editor (incorrect log?).";
		return Id();
	}
}
