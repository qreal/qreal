#include "visualInterpreterUnit.h"

#include <QEventLoop>
#include <QSet>

using namespace qReal;

VisualInterpreterUnit::VisualInterpreterUnit(
		qReal::LogicalModelAssistInterface const &logicalModelApi
		, qReal::GraphicalModelAssistInterface const &graphicalModelApi
		, qReal::gui::MainWindowInterpretersInterface &interpretersInterface)
		: BaseGraphTransformationUnit(logicalModelApi, graphicalModelApi, interpretersInterface)
		, mRuleParser(new RuleParser(logicalModelApi, graphicalModelApi, interpretersInterface.errorReporter()))
{
	defaultProperties.insert("semanticsStatus");
}

IdList VisualInterpreterUnit::getRules() const
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

void VisualInterpreterUnit::loadSemantics()
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
				Id const fromId = fromInRule(ruleElement);
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

	report(tr("Semantics loaded successfully"));
}

void VisualInterpreterUnit::interpret()
{
	int const timeout = SettingsManager::value("debuggerTimeout", 750).toInt();

	while (findMatch()) {
		makeStep();
		//highlightMatch();
		report(tr("Rule '") + mMatchedRuleName + tr("' was applied successfully"));

		pause(timeout);
	}

	report(tr("No rule cannot be applied"));
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
	if (mRules == NULL) {
		report(tr("Semantics not loaded"));
		return false;
	}

	foreach (QString const &ruleName, mRules->keys()) {
		mCurrentRuleName = ruleName;
		ruleToFind = mRules->value(ruleName);
		if (checkRuleMatching()) {
			mMatchedRuleName = ruleName;
			return true;
		}
	}
	
	return false;
}

Id VisualInterpreterUnit::getStartElement() const
{
	IdList const elementsInRule = children(ruleToFind);

	foreach (Id const &element, elementsInRule) {
		if (!isEdgeInRule(element) && element.element() != "ControlFlowMark") {
			return element;
		}
	}

	return Id::rootId();
}

void VisualInterpreterUnit::makeStep()
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
	QString const ruleProcess = getProperty(rule, "procedure").toString();
	if (ruleProcess != "") {
		mRuleParser->setRuleId(rule);
		mRuleParser->parseRule(ruleProcess, &firstMatch);
	}
	
	mMatches.clear();
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

Id VisualInterpreterUnit::getNodeIdWithControlMark(Id const &controlMarkId) const
{
	Id const link = outgoingLinks(controlMarkId).at(0);
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

void VisualInterpreterUnit::report(QString const &message) const
{
	mInterpretersInterface.errorReporter()->addInformation(message);
}
