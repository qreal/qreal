#include "graphTransformationUnit.h"

#include <QEventLoop>

using namespace qReal;

GraphTransformationUnit::GraphTransformationUnit(
		qReal::LogicalModelAssistInterface const &logicalModelApi
		, qReal::GraphicalModelAssistInterface const &graphicalModelApi
		, qReal::gui::MainWindowInterpretersInterface &interpretersInterface)
		: mInterpretersInterface(interpretersInterface)
		, mLogicalModelApi(logicalModelApi)
		, mGraphicalModelApi(graphicalModelApi)
{
}

GraphTransformationUnit::~GraphTransformationUnit()
{
}

IdList GraphTransformationUnit::getElementsFromActiveDiagram() const
{
	Id const activeDiagram = mInterpretersInterface.activeDiagram();
	return children(activeDiagram);
}

IdList GraphTransformationUnit::getRules() const
{
	IdList const elements = getElementsFromActiveDiagram();
	IdList result;
	foreach (Id const &element, elements) {
		if (element.element() == "SemanticsRule") {
			result.append(element);
		}
	}
	return result;
}

void GraphTransformationUnit::putIdIntoMap(QHash<QString, IdList*> *map,
		QString const &ruleName, Id const &id)
{
	if (!map->contains(ruleName)) {
		map->insert(ruleName, new IdList());
	}
	map->value(ruleName)->append(id);
}

bool GraphTransformationUnit::isSemanticsEditor()
{
	return mInterpretersInterface.activeDiagram().editor().contains("Semantics");
}

void GraphTransformationUnit::loadSemantics()
{
	if (!isSemanticsEditor()) {
		report("Current diagram is not for semantics. Select proper model.");
		return;
	}
	
	IdList const rules = getRules();
	
	mRules = new QHash<QString, Id>();
	mDeletedElements = new QHash<QString, IdList*>();
	mReplacedElements = new QHash<QString, IdList*>();
	mCreatedElements = new QHash<QString, IdList*>();
	mElementsWithNewControlMark = new QHash<QString, IdList*>();
	mElementsWithControlMark = new QHash<QString, IdList*>();
	
	foreach (Id const &rule, rules) {
		QString ruleName = getProperty(rule, "ruleName").toString();
		mRules->insert(ruleName, rule);
		IdList const ruleElements = children(rule);
		foreach (Id const &ruleElement, ruleElements) {
			if (ruleElement.element() == "ControlFlowLocation") {
				continue;
			}
			
			if (ruleElement.element() == "Replacement"){
				Id const fromId = from(ruleElement);
				putIdIntoMap(mReplacedElements, ruleName, fromId);
				continue;
			}
			
			QString const semanticsStatus = getProperty(ruleElement, "semanticsStatus").toString();
			if (ruleElement.element() == "ControlFlowMark") {
				if (semanticsStatus == "" || semanticsStatus == "@deleted@") {
					Id const nodeWithControl = getNodeIdWithControlMark(ruleElement);
					putIdIntoMap(mElementsWithControlMark, ruleName, nodeWithControl);
					if (semanticsStatus == "@deleted@") {
						putIdIntoMap(mDeletedElements, ruleName, ruleElement);
					}
				} else {
					putIdIntoMap(mCreatedElements, ruleName, ruleElement);
				}
				continue;
			}

			if (semanticsStatus == "@new@") {
				putIdIntoMap(mCreatedElements, ruleName, ruleElement);
			} else if (semanticsStatus == "@deleted@") {
				putIdIntoMap(mDeletedElements, ruleName, ruleElement);
			}
		}
	}
	
	report("Semantics loaded successfully");
}

void GraphTransformationUnit::interpret()
{
	if (findMatch()) {
		makeStep();
		highlightMatch();
		report("Rule '" + mMatchedRuleName + "' was applied successfully");
	} else {
		report("No rule cannot be applied");
	}
}

void GraphTransformationUnit::highlightMatch()
{
	foreach (Id const &id, mMatch->keys()) {
		mInterpretersInterface.highlight(mMatch->value(id), false);
	}
	
	QEventLoop loop;
	QTimer::singleShot(2000, &loop, SLOT(quit()));
	loop.exec();
	
	mInterpretersInterface.dehighlight();
}

bool GraphTransformationUnit::findMatch()
{
	if (mRules == NULL) {
		report("Semantics not loaded");
		return false;
	}
	
	foreach (QString const &ruleName, mRules->keys()) {
		if (checkRuleMatching(mRules->value(ruleName))) {
			mMatchedRuleName = ruleName;
			return true;
		}
	}
	return false;
}

bool GraphTransformationUnit::checkRuleMatching(Id const &rule)
{
	mMatch = new QHash<Id, Id>();
	
	Id const startElement = getStartElement(rule);
	mNodesHavingOutsideLinks.append(startElement);
	
	IdList const elements = getElementsFromActiveDiagram();
	foreach (Id const &element, elements) {
		if (compareElementTypes(startElement, element)) {
			mCurrentMatchedGraphInRule.clear();
			mNodesHavingOutsideLinks.clear();
			mMatch->clear();
			mPos = 0;
			
			mCurrentMatchedGraphInRule.append(startElement);
			mNodesHavingOutsideLinks.append(startElement);
			mMatch->insert(startElement, element);
			
			if (checkRuleMatchingRecursively()) {
				return true;
			}
		}
	}
	
	return false;
}

bool GraphTransformationUnit::checkRuleMatchingRecursively()
{
	if (mNodesHavingOutsideLinks.length() == mPos) {
		return true;
	}
	
	Id const nodeInRule = mNodesHavingOutsideLinks.at(mPos);
	Id const linkInRule = getOutsideLink(nodeInRule);
	
	if (linkInRule != Id::rootId()) {
		Id const linkEndInRule = getLinkEnd(linkInRule, nodeInRule);
		Id const nodeInModel = mMatch->value(nodeInRule);
		IdList const linksInModel = getProperLinks(nodeInModel, linkInRule);
		
		foreach (Id const &linkInModel, linksInModel) {
			Id const linkEndInModel = getLinkEnd(linkInModel, nodeInModel);
			if (checkNodeForAddingToMatch(linkEndInModel, linkEndInRule)) {
				if (checkRuleMatchingRecursively()) {
					return true;
				} else {
					rollback();
				}
			}
		}
		
		return false;
	} else {
		mPos++;
		return checkRuleMatchingRecursively();
	}
}

bool GraphTransformationUnit::checkNodeForAddingToMatch(Id const &nodeInModel,
		Id const &nodeInRule)
{
	QHash<Id, Id> *linksToAddToMatch = new QHash<Id, Id>();
	
	bool res = checkExistingLinks(nodeInModel, nodeInRule, linksToAddToMatch);
	
	if (res) {
		mMatch->unite(*linksToAddToMatch);
		mMatch->insert(nodeInRule, nodeInModel);
		mCurrentMatchedGraphInRule.append(nodeInRule);
		mNodesHavingOutsideLinks.append(nodeInRule);
	}
	
	return res;
}

bool GraphTransformationUnit::checkExistingLinks(Id const &nodeInModel,
		Id const &nodeInRule, QHash<Id, Id> *linksToAddInMatch)
{
	IdList const linksInRule = links(nodeInRule);
	
	foreach (Id const &linkInRule, linksInRule) {
		Id const linkEndInRule = getLinkEnd(linkInRule, nodeInRule);
		if (mCurrentMatchedGraphInRule.contains(linkEndInRule)) {
			Id const properLinkInModel = getProperLink(nodeInModel, linkInRule,
					linkEndInRule);
			if (properLinkInModel == Id::rootId()) {
				return false;
			} else {
				linksToAddInMatch->insert(linkInRule, properLinkInModel);
			}
		}
	}
	
	return true;
}

void GraphTransformationUnit::rollback()
{
	Id const nodeToRemove = mCurrentMatchedGraphInRule.last();
	
	mMatch->remove(nodeToRemove);
	mCurrentMatchedGraphInRule.removeLast();
	mNodesHavingOutsideLinks.removeLast();
	if (mPos == mNodesHavingOutsideLinks.length()) {
		mPos--;
	}
	
	IdList const links = getLinksToMatchedSubgraph(nodeToRemove);
	foreach (Id const &link, links) {
		mMatch->remove(link);
	}
}

IdList GraphTransformationUnit::getLinksToMatchedSubgraph(Id const &nodeInRule) const
{
	IdList result;
	IdList const linksInRule = links(nodeInRule);
	foreach (Id const &link, linksInRule) {
		Id const linkEnd = getLinkEnd(link, nodeInRule);
		if (mCurrentMatchedGraphInRule.contains(linkEnd)) {
			result.append(link);
		}
	}
	return result;
}

Id GraphTransformationUnit::getOutsideLink(Id const &nodeInRule) const
{
	IdList linksInRule = links(nodeInRule);
	foreach (Id const &linkInRule, linksInRule) {
		Id const linkEndInRule = getLinkEnd(linkInRule, nodeInRule);
		if (!mCurrentMatchedGraphInRule.contains(linkEndInRule)) {
			return linkInRule;
		}
	}
	
	return Id::rootId();
}

Id GraphTransformationUnit::getLinkEnd(Id const &linkInRule, Id const &nodeInRule) const
{
	Id const linkTo = to(linkInRule);
	if (linkTo == nodeInRule) {
		return from(linkInRule);
	}
	return linkTo;
}

Id GraphTransformationUnit::getProperLink(Id const &nodeInModel,
		 Id const &linkInRule, Id const &linkEndInRule) const
{
	IdList linksInModel = links(nodeInModel);
	foreach (Id const &linkInModel, linksInModel) {
		if (compareElements(linkInModel, linkInRule)) {
			Id const linkEndInModel = getLinkEnd(linkInModel, nodeInModel);
			if (linkEndInModel == mMatch->value(linkEndInRule)) {
				return linkInModel;
			}
		}
	}
	
	return Id::rootId();
}

IdList GraphTransformationUnit::getProperLinks(Id const &nodeInModel,
		Id const &linkInRule) const
{
	IdList result;
	IdList linksInModel = links(nodeInModel);
	foreach (Id const &linkInModel, linksInModel) {
		if (compareElements(linkInModel, linkInRule)) {
			result.append(linkInModel);
		}
	}
	return result;
}

Id GraphTransformationUnit::getStartElement(Id const &rule) const
{
	IdList const elementsInRule = children(rule);
	
	foreach (Id const &element, elementsInRule) {
		if (!isEdge(element)) {
			return element;
		}
	}
	
	return Id::rootId();
}

void GraphTransformationUnit::makeStep()
{
}

bool GraphTransformationUnit::compareElements(Id const &first,Id const &second) const
{
	bool result = compareElementTypes(first, second);
	if (isEdge(first)) {
		result = result && compareElementTypes(to(first), to(second))
				&& compareElementTypes(from(first), from(second));
	}
	return result;
}

bool GraphTransformationUnit::compareElementTypes(Id const &first, Id const &second) const
{
	return first.element() == second.element() && first.diagram() == second.diagram();
}

bool GraphTransformationUnit::isEdge(Id const &element) const
{
	return to(element) != Id::rootId() ||
			from(element) != Id::rootId();
}

QVariant GraphTransformationUnit::getProperty(Id const &id, QString const &propertyName)
{
	if (mLogicalModelApi.isLogicalId(id)) {
		return mLogicalModelApi.logicalRepoApi().property(id, propertyName);
	} else {
		return mLogicalModelApi.logicalRepoApi().property(
					mGraphicalModelApi.logicalId(id), propertyName);
	}
}

Id GraphTransformationUnit::getNodeIdWithControlMark(Id const &controlMarkId) const
{
	Id const link = outgoingLinks(controlMarkId).at(0);
	return to(link);
}

Id GraphTransformationUnit::to(Id const &id) const
{
	return mLogicalModelApi.logicalRepoApi().to(id);
}

Id GraphTransformationUnit::from(Id const &id) const
{
	return mLogicalModelApi.logicalRepoApi().from(id);
}

IdList GraphTransformationUnit::outgoingLinks(Id const &id) const
{
	return mLogicalModelApi.logicalRepoApi().outgoingLinks(id);
}

IdList GraphTransformationUnit::incomingLinks(Id const &id) const
{
	return mLogicalModelApi.logicalRepoApi().incomingLinks(id);
}

IdList GraphTransformationUnit::links(Id const &id) const
{
	return mLogicalModelApi.logicalRepoApi().links(id);
}

IdList GraphTransformationUnit::children(Id const &id) const
{
	return mGraphicalModelApi.graphicalRepoApi().children(id);
}

void GraphTransformationUnit::report(QString const &message) const
{
	mInterpretersInterface.errorReporter()->addInformation(message);
}
