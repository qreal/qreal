#include "rulesChecker.h"

using namespace qReal::rulesChecker;

RulesChecker::RulesChecker(qrRepo::GraphicalRepoApi const &graphicalRepoApi
		, qReal::gui::MainWindowInterpretersInterface &interpretersInterface)
	: mGRepoApi(&graphicalRepoApi)
	, mWindowInterface(&interpretersInterface)
	, hasNoErrors(true)
{
	linksTypes << "SequenceFlow";
}

void RulesChecker::makeDetour(Id const currentNode)
{
	if (isLink(currentNode))
	{
		Id destination = mGRepoApi->to(currentNode);
		if (destination == Id::rootId())
			postError(IncorrectLink, currentNode);
		else
			makeDetour(destination);

		metamodels.removeOne(currentNode);
		return;
	}

	IdList frontNodes = mGRepoApi->outgoingLinks(currentNode);
	if (!frontNodes.size() && currentNode.element() != QString("EndEvent"))
		postError(NoEndNode, currentNode);

	foreach (Id key, frontNodes) {
		makeDetour(key);
	}

	metamodels.removeOne(currentNode);
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
	metamodels = mGRepoApi->children(mWindowInterface->activeDiagram());

	researchDiagram();

	if (hasNoErrors)
		mWindowInterface->errorReporter()->addInformation(QString("Compiled without errors"));
}

qReal::IdList RulesChecker::findStartingElements(qReal::IdList const list)
{
	IdList headNodes;
	foreach (Id key, list) {
		if (key.type() == Id(key.editor(), key.diagram(), QString("StartEvent"))) {
			headNodes << key;
		}
	}
	return headNodes;
}

void RulesChecker::postError(RulesChecker::ErrorsType const error, Id const &badNode)
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
	hasNoErrors = false;
}

bool RulesChecker::isLink(const qReal::Id &model)
{
	return linksTypes.contains(model.element());
}

