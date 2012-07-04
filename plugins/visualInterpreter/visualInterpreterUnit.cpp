#include "visualInterpreterUnit.h"

#include <QEventLoop>
#include <QSet>

using namespace qReal;

VisualInterpreterUnit::VisualInterpreterUnit(
		qReal::LogicalModelAssistInterface const &logicalModelApi
		, qReal::GraphicalModelAssistInterface const &graphicalModelApi
		, qReal::gui::MainWindowInterpretersInterface &interpretersInterface)
		: BaseGraphTransformationUnit(logicalModelApi, graphicalModelApi, interpretersInterface)
		, isSemanticsLoaded(false)
		, mRuleParser(new RuleParser(logicalModelApi, graphicalModelApi, interpretersInterface.errorReporter()))
{
	mDefaultProperties.insert("semanticsStatus");
}

IdList VisualInterpreterUnit::allRules() const
{
	IdList const elements = elementsFromActiveDiagram();
	IdList result;
	foreach (Id const &element, elements) {
		if (element.element() == "SemanticsRule") {
			result.append(element);
		}
	}
	return result;
}

void VisualInterpreterUnit::putIdIntoMap(QHash<QString, IdList*> *map,
		QString const &ruleName, Id const &id)
{
	if (!map->contains(ruleName)) {
		map->insert(ruleName, new IdList());
	}
	map->value(ruleName)->append(id);
}

bool VisualInterpreterUnit::isSemanticsEditor()
{
	return mInterpretersInterface.activeDiagram().editor().contains("Semantics");
}

void VisualInterpreterUnit::initBeforeSemanticsLoading() {
	mRules = new QHash<QString, Id>();
	mDeletedElements = new QHash<QString, IdList*>();
	mReplacedElements = new QHash<QString, IdList*>();
	mCreatedElements = new QHash<QString, IdList*>();
	mNodesWithNewControlMark = new QHash<QString, IdList*>();
	mNodesWithDeletedControlMark = new QHash<QString, IdList*>();
	mNodesWithControlMark = new QHash<QString, IdList*>();
}

void VisualInterpreterUnit::initBeforeInterpretation() {
	mCurrentNodesWithControlMark.clear();
	mInterpretersInterface.dehighlight();
	mRuleParser->clear();
	mRuleParser->setErrorReporter(mInterpretersInterface.errorReporter());
	resetRuleSyntaxCheck();
}

void VisualInterpreterUnit::loadSemantics()
{
	if (!isSemanticsEditor()) {
		semanticsLoadingError(tr("Current diagram is not for semantics. Select proper model."));
		return;
	}

	IdList const rules = allRules();
	initBeforeSemanticsLoading();
	mInterpretersInterface.dehighlight();

	foreach (Id const &rule, rules) {
		QString const ruleName = property(rule, "ruleName").toString();
		if (ruleName == "") {
			semanticsLoadingError(tr("One of the rules doesn't have a name."));
			return;
		}
		
		IdList const ruleElements = children(rule);
		
		if (ruleElements.size() == 0) {
			semanticsLoadingError(tr("One of the rules is empty."));
			return;
		}
		
		mRules->insert(ruleName, rule);
		foreach (Id const &ruleElement, ruleElements) {
			if (ruleElement.element() == "ControlFlowLocation" ||
					ruleElement.element() == "Wildcard") {
				continue;
			}

			if (ruleElement.element() == "Replacement"){
				Id const fromId = fromInRule(ruleElement);
				putIdIntoMap(mReplacedElements, ruleName, fromId);
				continue;
			}

			QString const semanticsStatus = property(ruleElement, "semanticsStatus").toString();
			if (ruleElement.element() == "ControlFlowMark") {
				Id const nodeWithControl = nodeIdWithControlMark(ruleElement);
				
				if (nodeWithControl == Id::rootId()) {
					semanticsLoadingError(tr("Control flow mark in rule '")
							+ ruleName + tr("' isn't connected properly."));
					return;
				}
				
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

	isSemanticsLoaded = true;
	report(tr("Semantics loaded successfully"), false);
}

void VisualInterpreterUnit::interpret()
{
	if (!isSemanticsLoaded) {
		report(tr("Semantics not loaded"), true);
		return;
	}
	
	initBeforeInterpretation();
	int const timeout = SettingsManager::value("debuggerTimeout", 750).toInt();
	
	while (findMatch()) {
		if (hasRuleSyntaxError()) {
			report(tr("Rule '") +mMatchedRuleName +
					tr("' cannot be applied because semantics has syntax errors"), true);
			return;
		}
		
		if (!makeStep()) {
			report(tr("Rule '") +mMatchedRuleName + tr("' applying failed"), true);
			return;
		}
		
		report(tr("Rule '") + mMatchedRuleName + tr("' was applied successfully"), false);
		pause(timeout);
	}
	if (!hasRuleSyntaxError()) {
		report(tr("No rule cannot be applied"), false);
	}
}

void VisualInterpreterUnit::highlightMatch()
{
	foreach (Id const &id, mMatch->keys()) {
		mInterpretersInterface.highlight(mMatch->value(id), false);
	}

	pause(2000);

	mInterpretersInterface.dehighlight();
}

bool VisualInterpreterUnit::findMatch()
{
	foreach (QString const &ruleName, mRules->keys()) {
		mCurrentRuleName = ruleName;
		mRuleToFind = mRules->value(ruleName);
		if (checkRuleMatching()) {
			mMatchedRuleName = ruleName;
			return true;
		}
	}
	
	return false;
}

Id VisualInterpreterUnit::startElement() const
{
	IdList const elementsInRule = children(mRuleToFind);

	foreach (Id const &element, elementsInRule) {
		if (!isEdgeInRule(element) && element.element() != "ControlFlowMark") {
			return element;
		}
	}

	return Id::rootId();
}

bool VisualInterpreterUnit::makeStep()
{
	QHash<Id, Id> firstMatch = mMatches.at(0);
	
	if (mNodesWithDeletedControlMark->contains(mMatchedRuleName)) {
		foreach (Id const &id, *(mNodesWithDeletedControlMark->value(mMatchedRuleName))) {
			Id const node = firstMatch.value(id);
			mInterpretersInterface.dehighlight(node);
			mCurrentNodesWithControlMark.removeOne(node);
		}
	}

	if (mNodesWithNewControlMark->contains(mMatchedRuleName)) {
		foreach (Id const &id, *(mNodesWithNewControlMark->value(mMatchedRuleName))) {
			Id const node = firstMatch.value(id);
			mInterpretersInterface.highlight(node, false);
			mCurrentNodesWithControlMark.append(node);
		}
	}

	Id const rule = mRules->value(mMatchedRuleName);
	QString const ruleProcess = property(rule, "procedure").toString();
	bool result = true;
	if (ruleProcess != "") {
		mRuleParser->setRuleId(rule);
		result = mRuleParser->parseRule(ruleProcess, &firstMatch);
	}
	
	mMatches.clear();
	return result;
}

bool VisualInterpreterUnit::compareElements(Id const &first, Id const &second) const
{
	bool result = BaseGraphTransformationUnit::compareElements(first, second);

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

bool VisualInterpreterUnit::compareElementTypesAndProperties(Id const &first,
		Id const &second) const
{
	if (second.element() == "Wildcard") {
		return true;
	}

	return BaseGraphTransformationUnit::compareElementTypesAndProperties(first,
			second);
}

Id VisualInterpreterUnit::nodeIdWithControlMark(Id const &controlMarkId) const
{
	IdList const outLinks = outgoingLinks(controlMarkId);
	if (outLinks.size() == 0) {
		return Id::rootId();
	}
	
	Id const link = outLinks.at(0);
	return toInRule(link);
}

IdList VisualInterpreterUnit::linksInRule(Id const &id) const
{
	IdList result;
	foreach (Id const &id, mLogicalModelApi.logicalRepoApi().links(id)) {
		if (id.element() != "Replacement" && id.element() != "ControlFlowLocation") {
			result.append(id);
		}
	}

	return result;
}

void VisualInterpreterUnit::semanticsLoadingError(QString const &message)
{
	report(message + tr(" Semantics loading failed."), true);
	isSemanticsLoaded = false;
}
