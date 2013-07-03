#include "rulesChecker.h"

using namespace qReal::rulesChecker;

RulesChecker::RulesChecker(qrRepo::GraphicalRepoApi const &graphicalRepoApi
		, qReal::gui::MainWindowInterpretersInterface &interpretersInterface)
	: mGRepoApi(&graphicalRepoApi)
	, mWindowInterface(&interpretersInterface)
	, hasNoErrors(true)
{
	linkTypes << "SequenceFlow";
	containerTypes << "Pool" << "Lane" << "BPMN Diagram";
}

void RulesChecker::makeDetour(Id const currentNode)
{
	if (!metamodels.removeOne(currentNode))
		return;

	if (isLink(currentNode))
	{
		Id destination = mGRepoApi->to(currentNode);
		if (destination == Id::rootId())
			postError(IncorrectLink, currentNode);
		else
			makeDetour(destination);
		return;
	}

	IdList frontNodes = mGRepoApi->outgoingLinks(currentNode);
	if (!frontNodes.size() && currentNode.element() != QString("EndEvent"))
		postError(NoEndNode, currentNode);

	foreach (Id key, frontNodes) {
		makeDetour(key);
	}
}

void RulesChecker::findIncorrectLinks()
{
	foreach (Id key, metamodels) {
		if (isLink(key) && (mGRepoApi->to(key) == Id::rootId() || mGRepoApi->from(key) == Id::rootId()))
			postError(IncorrectLink, key);
	}
}

void RulesChecker::researchDiagram()
{
	IdList startingElements = findStartingElements(metamodels);

	while (startingElements.size()) {
		makeDetour(startingElements.first());
		startingElements.removeFirst();
	}

	if (metamodels.size() && !startingElements.size()) {
		postError(NoStartNode, metamodels.first());
		findIncorrectLinks();
	}
}

void RulesChecker::check()
{
	hasNoErrors = true;
	mWindowInterface->dehighlight();
	mWindowInterface->errorReporter()->clear();
	metamodels = mGRepoApi->children(Id::rootId());
	//metamodels = mGRepoApi->children(mWindowInterface->activeDiagram());

	researchDiagram();

	if (hasNoErrors)
		mWindowInterface->errorReporter()->addInformation(QString("Compiled without errors"));
}

qReal::IdList RulesChecker::findStartingElements(qReal::IdList &list)
{
	IdList headNodes;
	foreach (Id const key, list) {
		if (containerTypes.contains(key.element()))
			list.removeOne(key);
		if (key.element() == QString("StartEvent")) {
			headNodes << key;
		}
	}
	return headNodes;
}

void RulesChecker::postError(RulesChecker::ErrorsType const error, Id badNode)
{
	QString errorMsg("");
	switch (error) {
	case NoStartNode: {
		errorMsg = "There is no start-node in path";
		break;
	}
	case NoEndNode: {
		errorMsg = "There is no end-node in path";
		break;
	}
	case IncorrectLink: {
		errorMsg = "Some links are incorrect";
		break;
	}
	}
	mWindowInterface->errorReporter()->addError(errorMsg, badNode);
	mWindowInterface->highlight(badNode, false);
	qDebug() << badNode.toUrl();
	hasNoErrors = false;
}

bool RulesChecker::isLink(const qReal::Id &model)
{
	return linkTypes.contains(model.element());
}

