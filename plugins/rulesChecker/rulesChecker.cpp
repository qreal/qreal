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

bool RulesChecker::makeDetour(qReal::Id const currentNode, IdList &usedNodes)
{
	if (usedNodes.contains(currentNode))
		return false; // cannot learn some more here

	if (!mDiagramModels.contains(currentNode))
		return true;  // we already have made detour of forward nodes

	mDiagramModels.removeOne(currentNode);
	usedNodes.append(currentNode);

	if (isLink(currentNode))
	{
		Id destination = mGRepoApi->to(currentNode);
		if (destination == Id::rootId()) {
			postError(NoEndNode, currentNode); // we've already put info that link is incorrect
			return true; // done end-job for link(50%)
		}
		return makeDetour(destination, usedNodes);
	}

	if (isEndNode(currentNode))
		return true; // we found real end-node

	IdList frontNodes =  mGRepoApi->outgoingLinks(currentNode);
	if (!frontNodes.size()) {
		postError(NoEndNode, currentNode);
		return true; // done end-job for nodes (now 100%)
	}

	bool hasFoundFinalNode = false; // to catch that we have found end-node anywhere in path
	foreach (Id key, frontNodes) {
		if (makeDetour(key, usedNodes))
			hasFoundFinalNode = true;
	}
	return hasFoundFinalNode;
}

void RulesChecker::checkLinksRule(qReal::Id const &key)
{
	if (isLink(key))
		if (mGRepoApi->from(key) == Id::rootId() || mGRepoApi->to(key) == Id::rootId())
			postError(IncorrectLink, key);
}

void RulesChecker::checkFinalNodeRule(const qReal::Id &key)
{
	bool isLastNode = isEndNode(key);
	if (!isLastNode && !isStartNode(key))
		return;

	IdList incorrectLinks = ((isLastNode)? mGRepoApi->outgoingLinks(key) : mGRepoApi->incomingLinks(key));
	if (incorrectLinks.size()) {
		postError(((isLastNode)? LinkFromFinalNode : LinkToStartNode), key);
		foreach (Id const key, incorrectLinks) {
			mDiagramModels.removeOne(key);
		}
	}
}

void RulesChecker::researchDiagram()
{
	IdList startingElements = checkDiagramModelsList(mDiagramModels);

	// check all paths which have starting nodes
	while (startingElements.size()) {
		Id const currentHead = startingElements.first();
		IdList usedNodes;
		if (!makeDetour(currentHead, usedNodes))
			postError(NoEndNode, startingElements.first());
		startingElements.removeFirst();
	}

	// check other connected components
	while (mDiagramModels.size()) {
		Id headNode = findFirstNode(mDiagramModels.first());
		postError(NoStartNode, headNode);
		IdList usedNodes;
		if (!makeDetour(headNode, usedNodes))
			postError(NoEndNode, headNode);
	}
}

void RulesChecker::prepareOutput()
{
	hasNoErrors = true;
	mWindowInterface->dehighlight();
	mWindowInterface->errorReporter()->clear();
}

void RulesChecker::checkAllDiagrams()
{
	prepareOutput();

	IdList diagrams = mGRepoApi->graphicalElements(Id(QString("BPMNDiagram")
			, QString("BPMNMetamodel"), QString("BPMN Diagram")));

	foreach (Id const diagram, diagrams) {
		mDiagramModels = elementsOfDiagram(diagram);
		researchDiagram();
	}

	if (hasNoErrors)
		mWindowInterface->errorReporter()->addInformation(QString("All diagrams compiled without errors"));
}

void RulesChecker::checkCurrentDiagram()
{
	prepareOutput();
	mDiagramModels = elementsOfDiagram(mWindowInterface->activeDiagram());
	researchDiagram();

	if (hasNoErrors)
		mWindowInterface->errorReporter()->addInformation(QString("Current diagram compiled without errors"));
}

void RulesChecker::postError(RulesChecker::ErrorsType const error, Id badNode)
{
	QString errorMsg("");
	switch (error) {
	case LinkToStartNode: {
		errorMsg = "There are links to start node";
		break;
	}
	case LinkFromFinalNode: {
		errorMsg = "There are links from End-event";
		break;
	}
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
	default: {
		errorMsg = "There are problems";
	}
	}
	mWindowInterface->errorReporter()->addError(errorMsg, badNode);
	mWindowInterface->highlight(badNode, false);
	hasNoErrors = false;
}

bool RulesChecker::isLink(qReal::Id const &model) const
{
	return linkTypes.contains(model.element());
}

bool RulesChecker::isStartNode(qReal::Id const &model) const
{
	return (model.element() == "StartEvent");
}

bool RulesChecker::isEndNode(qReal::Id const &model) const
{
	return (model.element() == "EndEvent");
}

qReal::IdList RulesChecker::elementsOfDiagram(qReal::Id const &diagram) const
{
	IdList result = mGRepoApi->children(diagram);
	int const childrenCount = result.size();
	for (int i = 0; i < childrenCount; i++)
		result.append(elementsOfDiagram(result.at(i)));

	return result;
}

qReal::IdList RulesChecker::checkDiagramModelsList(qReal::IdList &list)
{
	IdList headNodes;
	foreach (Id const key, list) {
		if (containerTypes.contains(key.element()))
			list.removeOne(key);
		if (isStartNode(key)) {
			headNodes << key;
		}
		checkLinksRule(key);
		checkFinalNodeRule(key);
	}
	return headNodes;
}

qReal::Id RulesChecker::findFirstNode(qReal::Id const &key) const
{
	IdList startingList;
	getPreviousNodes(key, startingList);

	Id result = startingList.first();
	int minIncomingLinks = mGRepoApi->incomingLinks(result).size();

	foreach (Id const key, startingList) {
		int incomingLinks = mGRepoApi->incomingLinks(key).size();
		if (incomingLinks < minIncomingLinks) {
			minIncomingLinks = incomingLinks;
			result = key;
		}
	}

	return result;
}

void RulesChecker::getPreviousNodes(qReal::Id const &key, IdList &result) const
{
	if (result.contains(key) || !mDiagramModels.contains(key)) // we wont loooking what already done
		return;

	result << key;
	foreach (Id const link, mGRepoApi->incomingLinks(key)) {
		getPreviousNodes(mGRepoApi->from(link), result);
	}
}

