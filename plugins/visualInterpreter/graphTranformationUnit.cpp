#include "graphTransformationUnit.h"

#include <QEventLoop>
#include <QSet>

using namespace qReal;

QSet<QString> const defaultProperties = (QSet<QString>()
		<< "from" << "incomingConnections" << "incomingUsages" << "links"
		<< "name" << "outgoingConnections" << "outgoingUsages" << "to"
		<< "semanticsStatus");

GraphTransformationUnit::GraphTransformationUnit(
		qReal::LogicalModelAssistInterface const &logicalModelApi
		, qReal::GraphicalModelAssistInterface const &graphicalModelApi
		, qReal::gui::MainWindowInterpretersInterface &interpretersInterface)
		: mInterpretersInterface(interpretersInterface)
		, mLogicalModelApi(logicalModelApi)
		, mGraphicalModelApi(graphicalModelApi)
		, mRuleParser(new RuleParser(logicalModelApi, graphicalModelApi, interpretersInterface.errorReporter()))
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
	mNodesWithNewControlMark = new QHash<QString, IdList*>();
	mNodesWithDeletedControlMark = new QHash<QString, IdList*>();
	mNodesWithControlMark = new QHash<QString, IdList*>();

	mInterpretersInterface.dehighlight();

	foreach (Id const &rule, rules) {
		QString const ruleName = getProperty(rule, "ruleName").toString();
		mRules->insert(ruleName, rule);
		IdList const ruleElements = children(rule);
		foreach (Id const &ruleElement, ruleElements) {
			if (ruleElement.element() == "ControlFlowLocation" ||
					ruleElement.element() == "Wildcard") {
				continue;
			}

			if (ruleElement.element() == "Replacement"){
				Id const fromId = from(ruleElement);
				putIdIntoMap(mReplacedElements, ruleName, fromId);
				continue;
			}

			QString const semanticsStatus = getProperty(ruleElement, "semanticsStatus").toString();
			if (ruleElement.element() == "ControlFlowMark") {
				Id const nodeWithControl = getNodeIdWithControlMark(ruleElement);
				if (semanticsStatus == "" || semanticsStatus == "@deleted@") {
					putIdIntoMap(mNodesWithControlMark, ruleName, nodeWithControl);
					if (semanticsStatus == "@deleted@") {
						putIdIntoMap(mNodesWithDeletedControlMark, ruleName,
								nodeWithControl);
					}
				} else {
					putIdIntoMap(mCreatedElements, ruleName, ruleElement);
					putIdIntoMap(mNodesWithNewControlMark, ruleName,
							nodeWithControl);
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

void GraphTransformationUnit::pause(int time)
{
	QEventLoop loop;
	QTimer::singleShot(time, &loop, SLOT(quit()));
	loop.exec();
}

void GraphTransformationUnit::interpret()
{
	int const timeout = SettingsManager::value("debuggerTimeout", 750).toInt();

	while (findMatch()) {
		makeStep();
		//highlightMatch();
		report("Rule '" + mMatchedRuleName + "' was applied successfully");

		pause(timeout);
	}

	report("No rule cannot be applied");
}

void GraphTransformationUnit::highlightMatch()
{
	foreach (Id const &id, mMatch->keys()) {
		QColor const color = QColor(SettingsManager::value("debugColor"
				, "red").toString());
		mInterpretersInterface.highlight(mMatch->value(id), false, color);
	}

	pause(2000);

	mInterpretersInterface.dehighlight();
}

bool GraphTransformationUnit::findMatch()
{
	if (mRules == NULL) {
		report("Semantics not loaded");
		return false;
	}

	foreach (QString const &ruleName, mRules->keys()) {
		mCurrentRuleName = ruleName;
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
		if (compareElements(element, startElement)) {
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
	IdList const linksInRule = links(nodeInRule);
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
	IdList const linksInModel = links(nodeInModel);
	foreach (Id const &linkInModel, linksInModel) {
		if (compareLinks(linkInModel, linkInRule)) {
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
	IdList const linksInModel = links(nodeInModel);
	foreach (Id const &linkInModel, linksInModel) {
		if (compareLinks(linkInModel, linkInRule)) {
			result.append(linkInModel);
		}
	}
	return result;
}

Id GraphTransformationUnit::getStartElement(Id const &rule) const
{
	IdList const elementsInRule = children(rule);

	foreach (Id const &element, elementsInRule) {
		if (!isEdge(element) && element.element() != "ControlFlowMark") {
			return element;
		}
	}

	return Id::rootId();
}

void GraphTransformationUnit::makeStep()
{
	if (mNodesWithDeletedControlMark->contains(mMatchedRuleName)) {
		foreach (Id const &id, *(mNodesWithDeletedControlMark->value(mMatchedRuleName))) {
			Id const node = mMatch->value(id);
			mInterpretersInterface.dehighlight(node);
			mCurrentNodesWithControlMark.removeOne(node);
		}
	}

	if (mNodesWithNewControlMark->contains(mMatchedRuleName)) {
		foreach (Id const &id, *(mNodesWithNewControlMark->value(mMatchedRuleName))) {
			Id const node = mMatch->value(id);
			QColor const color = QColor(SettingsManager::value("debugColor"
					, "red").toString());
			mInterpretersInterface.highlight(node, false, color);
			mCurrentNodesWithControlMark.append(node);
		}
	}

	Id const rule = mRules->value(mMatchedRuleName);
	QString const ruleProcess = getProperty(rule, "procedure").toString();
	if (ruleProcess != "") {
		mRuleParser->setRuleId(mRules->value(mMatchedRuleName));
		mRuleParser->parseRule(ruleProcess, mMatch);
	}
}

bool GraphTransformationUnit::compareLinks(Id const &first,Id const &second) const
{
	return compareElementTypesAndProperties(first, second)
			&& compareElements(to(first), to(second))
			&& compareElements(from(first), from(second));
}

bool GraphTransformationUnit::compareElements(Id const &first, Id const &second) const
{
	bool result = compareElementTypesAndProperties(first, second);

	if (mNodesWithControlMark->contains(mCurrentRuleName)) {
		if (mNodesWithControlMark->value(mCurrentRuleName)->contains(second)) {
			result = result && mCurrentNodesWithControlMark.contains(first);
		}
		if (mCurrentNodesWithControlMark.contains(first)) {
			result = result && mNodesWithControlMark->value(mCurrentRuleName)->contains(second);
		}
	} else if (result && mCurrentNodesWithControlMark.contains(first)) {
		return false;
	}

	return result;
}

bool GraphTransformationUnit::compareElementTypesAndProperties(Id const &first,
		Id const &second) const
{
	if (second.element() == "Wildcard") {
		return true;
	}

	if (first.element() == second.element() && first.diagram() == second.diagram()) {
		QHash<QString, QVariant> secondProperties = getProperties(second);

		foreach (QString const &key, secondProperties.keys()) {
			QVariant const value = secondProperties.value(key);

			if (value.toString() == "") {
				continue;
			}

			if (!hasProperty(first, key) || getProperty(first, key) != value) {
				return false;
			}
		}

		return true;
	}

	return false;
}

bool GraphTransformationUnit::hasProperty(Id const &id, QString const &propertyName) const
{
	if (mLogicalModelApi.isLogicalId(id)) {
		return mLogicalModelApi.logicalRepoApi().hasProperty(id, propertyName);
	} else {
		return mLogicalModelApi.logicalRepoApi().hasProperty(
				mGraphicalModelApi.logicalId(id), propertyName);
	}
}

QHash<QString, QVariant> GraphTransformationUnit::getProperties(Id const &id) const
{
	QHash<QString, QVariant> res;

	QMapIterator<QString, QVariant> properties =
			(mLogicalModelApi.isLogicalId(id))
			? mLogicalModelApi.logicalRepoApi().propertiesIterator(id)
			: mLogicalModelApi.logicalRepoApi().propertiesIterator(
					mGraphicalModelApi.logicalId(id));

	while (properties.hasNext()) {
		properties.next();

		if (!defaultProperties.contains(properties.key())) {
			res.insert(properties.key(), properties.value());
		}
	}

	return res;
}

bool GraphTransformationUnit::isEdge(Id const &element) const
{
	return to(element) != Id::rootId() ||
			from(element) != Id::rootId();
}

QVariant GraphTransformationUnit::getProperty(Id const &id, QString const &propertyName) const
{
	if (mLogicalModelApi.isLogicalId(id)) {
		return mLogicalModelApi.logicalRepoApi().property(id, propertyName);
	} else {
		return mLogicalModelApi.logicalRepoApi().property(
				mGraphicalModelApi.logicalId(id), propertyName);
	}
}

void GraphTransformationUnit::setProperty(Id const &id, QString const &propertyName
		, QVariant const &value) const
{
	if (mLogicalModelApi.isLogicalId(id)) {
		return mLogicalModelApi.logicalRepoApi().setProperty(id, propertyName, value);
	} else {
		return mLogicalModelApi.logicalRepoApi().setProperty(
				mGraphicalModelApi.logicalId(id), propertyName, value);
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
	IdList result;
	foreach (Id const &id, mLogicalModelApi.logicalRepoApi().links(id)) {
		if (id.element() != "Replacement" && id.element() != "ControlFlowLocation") {
			result.append(id);
		}
	}

	return result;
}

IdList GraphTransformationUnit::children(Id const &id) const
{
	return mGraphicalModelApi.graphicalRepoApi().children(id);
}

void GraphTransformationUnit::report(QString const &message) const
{
	mInterpretersInterface.errorReporter()->addInformation(message);
}
