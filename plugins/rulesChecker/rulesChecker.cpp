#include "rulesChecker.h"

using namespace qReal::rulesChecker;

RulesChecker::RulesChecker(qrRepo::GraphicalRepoApi const &graphicalRepoApi
		, qReal::gui::MainWindowInterpretersInterface &interpretersInterface)
	: mGRepoApi(&graphicalRepoApi)
	, mWindowInterface(&interpretersInterface)
	, mHasNoErrors(true)
{
	mLinkTypes << "SequenceFlow";
	mContainerTypes << "Pool" << "Lane" << "BPMN Diagram";
}

bool RulesChecker::makeDetour(Id const &currentNode, IdList &usedNodes)
{
	if (usedNodes.contains(currentNode)) {
		return false; // cannot learn some more here
	}

	if (!mDiagramModels.contains(currentNode)) {
		return true;  // we already have made detour of forward nodes
	}

	mDiagramModels.removeOne(currentNode);
	usedNodes.append(currentNode);

	if (isLink(currentNode)) {
		Id destination = mGRepoApi->to(currentNode);
		if (destination == Id::rootId()) {
			postError(NoEndNode, currentNode); // we've already put info that link is incorrect
			return true; // done end-job for link(50%)
		}
		return makeDetour(destination, usedNodes);
	}

	if (isEndNode(currentNode)) {
		return true; // we found real end-node
	}

	IdList frontNodes =  mGRepoApi->outgoingLinks(currentNode);
	if (isEmptyList(frontNodes)) {
		postError(NoEndNode, currentNode);
		return true; // done end-job for nodes (now 100%)
	}

	bool hasFoundFinalNode = false; // to catch that we have found end-node anywhere in path
	foreach (Id key, frontNodes) {
		if (makeDetour(key, usedNodes)) {
			hasFoundFinalNode = true;
		}
	}
	return hasFoundFinalNode;
}

void RulesChecker::checkLinksRule(qReal::Id const &key)
{
	if (isLink(key)) {
		if (mGRepoApi->from(key) == Id::rootId() || mGRepoApi->to(key) == Id::rootId()) {
			postError(IncorrectLink, key);
		}
	}
}

void RulesChecker::checkFinalNodeRule(qReal::Id const &key)
{
	bool isLastNode = isEndNode(key);
	if (!isLastNode && !isStartNode(key)) {
		return;
	}

	IdList incorrectLinks = (isLastNode) ? mGRepoApi->outgoingLinks(key) : mGRepoApi->incomingLinks(key);
	if (!isEmptyList(incorrectLinks)) {
		postError((isLastNode) ? LinkFromFinalNode : LinkToStartNode, key);
		foreach (Id const &key, incorrectLinks) {
			mDiagramModels.removeOne(key);
		}
	}
}

void RulesChecker::researchDiagram()
{
	checkDiagramModelsList();
	IdList startingElements = collectStartNodes();

	// check all paths which have starting nodes
	while (!isEmptyList(startingElements)) {
		Id const currentHead = startingElements.first();
		IdList usedNodes;
		if (!makeDetour(currentHead, usedNodes)) {
			postError(NoEndNode, startingElements.first());
		}
		startingElements.removeFirst();
	}

	// check other connected components
	while (!isEmptyList(mDiagramModels)) {
		Id headNode = findFirstNode(mDiagramModels.first());
		postError(NoStartNode, headNode);
		IdList usedNodes;
		if (!makeDetour(headNode, usedNodes)) {
			postError(NoEndNode, headNode);
		}
	}
}

void RulesChecker::prepareOutput()
{
	mHasNoErrors = true;
	mWindowInterface->dehighlight();
	mWindowInterface->errorReporter()->clear();
}

void RulesChecker::checkAllDiagrams()
{
	prepareOutput();

	IdList diagrams = mGRepoApi->graphicalElements(Id(QString("BPMNDiagram")
			, QString("BPMNMetamodel"), QString("BPMN Diagram")));

	foreach (Id const &diagram, diagrams) {
		mDiagramModels = elementsOfDiagram(diagram);
		researchDiagram();
	}

	if (mHasNoErrors) {
		mWindowInterface->errorReporter()->addInformation(QString("All diagrams compiled without errors"));
	}
}

void RulesChecker::checkCurrentDiagram()
{
	prepareOutput();
	mDiagramModels = elementsOfDiagram(mWindowInterface->activeDiagram());
	researchDiagram();

	if (mHasNoErrors) {
		mWindowInterface->errorReporter()->addInformation(QString("Current diagram compiled without errors"));
	}
}

void RulesChecker::postError(RulesChecker::ErrorsType const error, Id const &badNode)
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
	mHasNoErrors = false;
}

bool RulesChecker::isLink(qReal::Id const &node) const
{
	return mLinkTypes.contains(node.element());
}

bool RulesChecker::isContainer(const qReal::Id &node) const
{
	return mContainerTypes.contains(node.element());
}

bool RulesChecker::isStartNode(qReal::Id const &node) const
{
	return (node.element() == "StartEvent");
}

bool RulesChecker::isEndNode(qReal::Id const &node) const
{
	return (node.element() == "EndEvent");
}

bool RulesChecker::isEmptyList(qReal::IdList const &list) const
{
	return list.size() == 0;
}

qReal::IdList RulesChecker::elementsOfDiagram(qReal::Id const &diagram) const
{
	IdList result = mGRepoApi->children(diagram);
	int const childrenCount = result.size();
	for (int i = 0; i < childrenCount; i++) {
		result.append(elementsOfDiagram(result.at(i)));
	}

	return result;
}

void RulesChecker::checkDiagramModelsList()
{
	foreach (Id const &key, mDiagramModels) {
		if (isContainer(key)) {
			mDiagramModels.removeOne(key);
		}

		checkLinksRule(key);
		checkFinalNodeRule(key);
	}
}

qReal::IdList RulesChecker::collectStartNodes() const
{
	IdList headNodes;
	foreach (Id const &key, mDiagramModels) {
		if (isStartNode(key)) {
			headNodes << key;
		}
	}
	return headNodes;
}

qReal::Id RulesChecker::findFirstNode(qReal::Id const &key) const
{
	IdList startingList;
	getPreviousNodes(key, startingList);

	Id result = startingList.first();
	int minIncomingLinks = mGRepoApi->incomingLinks(result).size();

	foreach (Id const &key, startingList) {
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
	if (result.contains(key) || !mDiagramModels.contains(key)) { // we wont loooking what already done
		return;
	}

	result << key;
	foreach (Id const &link, mGRepoApi->incomingLinks(key)) {
		getPreviousNodes(mGRepoApi->from(link), result);
	}
}

