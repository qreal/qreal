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

bool RulesChecker::makeDetour(Id const currentNode, IdList &used, bool const goForwards)
{
	bool hasFoundFinalNode = false;
	if (used.contains(currentNode) && used.size() > 1)
		return false; //we already were here
	else
		used.append(currentNode);

	mDiagramModels.removeOne(currentNode);

	if (isLink(currentNode))
	{
		Id destination = ((goForwards)? mGRepoApi->to(currentNode) : mGRepoApi->from(currentNode));
		if (destination == Id::rootId())
			postError(IncorrectLink, currentNode);
		else
			return makeDetour(destination, used, goForwards);
	}

	IdList frontNodes = ((goForwards)? mGRepoApi->outgoingLinks(currentNode) : mGRepoApi->incomingLinks(currentNode));
	if (!frontNodes.size()) {
		QString finalNodeType = ((goForwards)? "EndEvent" : "StartEvent");
		if (currentNode.element() != finalNodeType)
			postError(((goForwards)? NoEndNode : NoStartNode), currentNode);

		hasFoundFinalNode = true;  // because found error
	}

	if (isValidFinalNode(currentNode))
		return true;

	foreach (Id key, frontNodes) {
		if (makeDetour(key, used, goForwards))
			hasFoundFinalNode = true;
	}
	return hasFoundFinalNode;
}


bool RulesChecker::isValidFinalNode(qReal::Id const &node)
{
	if (node.element() == "EndEvent" && mGRepoApi->outgoingLinks(node).size()) {
		postError(LinkFromFinalNode, node);
		return false;
	}
	if (node.element() == "StartEvent" && mGRepoApi->incomingLinks(node).size()) {
		postError(LinkToStartNode, node);
		return false;
	}
	return true;
}

void RulesChecker::researchDiagram()
{
	while (mDiagramModels.size()) {
		Id const currentHead = mDiagramModels.first();
		if (containerTypes.contains(currentHead.element())) {
			mDiagramModels.removeFirst();
			continue;
		}

		IdList used;
		if (!makeDetour(currentHead, used))
			postError(NoEndNode, currentHead);
		used.clear();
		used.append(currentHead);
		if (!makeDetour(currentHead, used, false))
			postError(NoStartNode, currentHead);
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
		mDiagramModels = childrenOfDiagram(diagram);
		researchDiagram();
	}

	if (hasNoErrors)
		mWindowInterface->errorReporter()->addInformation(QString("All diagrams compiled without errors"));
}

void RulesChecker::checkCurrentDiagram()
{
	prepareOutput();
	mDiagramModels = childrenOfDiagram(mWindowInterface->activeDiagram());
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

bool RulesChecker::isLink(const qReal::Id &model)
{
	return linkTypes.contains(model.element());
}

qReal::IdList RulesChecker::childrenOfDiagram(const qReal::Id &diagram)
{
	IdList result = mGRepoApi->children(diagram);
	int const childrenCount = result.size();
	for (int i = 0; i < childrenCount; i++)
		result.append(childrenOfDiagram(result.at(i)));

	return result;
}

