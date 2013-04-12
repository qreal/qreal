#include "visualInterpreterUnit.h"

using namespace qReal;

VisualInterpreterUnit::VisualInterpreterUnit(
		qReal::LogicalModelAssistInterface &logicalModelApi
		, qReal::GraphicalModelAssistInterface &graphicalModelApi
		, qReal::gui::MainWindowInterpretersInterface &interpretersInterface)
		: BaseGraphTransformationUnit(logicalModelApi, graphicalModelApi, interpretersInterface)
		, mIsSemanticsLoaded(false)
		, mNeedToStopInterpretation(false)
		, mIsInterpretationalSemantics(true)
		, mRules()
		, mRuleParser(new RuleParser(logicalModelApi, graphicalModelApi, interpretersInterface.errorReporter()))
		, mPythonGenerator(new PythonGenerator(logicalModelApi, graphicalModelApi, interpretersInterface))
		, mPythonInterpreter(new PythonInterpreter(this))
{
	mDefaultProperties.insert("semanticsStatus");
	mDefaultProperties.insert("id");
	connect(mPythonInterpreter, SIGNAL(readyReadStdOutput(QHash<QPair<QString, QString>, QString>))
			, this, SLOT(processPythonInterpreterStdOutput(QHash<QPair<QString, QString>, QString>)));
	connect(mPythonInterpreter, SIGNAL(readyReadErrOutput(QString))
			, this, SLOT(processPythonInterpreterErrOutput(QString)));
}

VisualInterpreterUnit::~VisualInterpreterUnit()
{
	delete mPythonGenerator;
	delete mPythonInterpreter;
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

void VisualInterpreterUnit::putIdIntoMap(QHash<QString, IdList*> *map, QString const &ruleName, Id const &id)
{
	if (!map->contains(ruleName)) {
		map->insert(ruleName, new IdList());
	}
	map->value(ruleName)->append(id);
}

bool VisualInterpreterUnit::isSemanticsEditor() const
{
	return mInterpretersInterface.activeDiagram().editor().contains("Semantics");
}

bool VisualInterpreterUnit::checkRuleMatching()
{
	if (mNodesWithControlMark.contains(mCurrentRuleName)) {
		IdList const elements = mCurrentNodesWithControlMark;
		return BaseGraphTransformationUnit::checkRuleMatching(elements);
	} else {
		IdList const elements = elementsFromActiveDiagram();
		return BaseGraphTransformationUnit::checkRuleMatching(elements);
	}
}

void VisualInterpreterUnit::initBeforeSemanticsLoading()
{
	mRules.clear();
	mDeletedElements.clear();
	mReplacedElements.clear();
	mCreatedElements.clear();
	mNodesWithNewControlMark.clear();
	mNodesWithDeletedControlMark.clear();
	mNodesWithControlMark.clear();
	mNeedToStopInterpretation = false;
	mInitializationCode = QPair<QString, QString>();
	mOrderedRules.clear();
}

void VisualInterpreterUnit::orderRulesByPriority()
{
	IdList rules = allRules();
	QList<int> priorities;
	foreach (Id const &rule, rules) {
		mOrderedRules.append(property(rule, "ruleName").toString());
		priorities.append(property(rule, "priority").toInt());
	}
	int n = mOrderedRules.length();
	for (int i = 1; i < n; i++) {
		bool flag = false;
		for (int j = 0; j < n - i; j++) {
			if (priorities.at(j) < priorities.at(j + 1)) {
				int const temp1 = priorities.at(j);
				priorities.removeAt(j);
				priorities.insert(j + 1, temp1);

				QString const temp2 = mOrderedRules.at(j);
				mOrderedRules.removeAt(j);
				mOrderedRules.insert(j + 1, temp2);

				flag = true;
			}
		}
		if (!flag) {
			break;
		}
	}
}

void VisualInterpreterUnit::readInitialization()
{
	IdList const elements = elementsFromActiveDiagram();
	foreach (Id const &element, elements) {
		if (element.element() == "Initialization") {
			mInitializationCode.first = property(element, "languageType").toString();
			mInitializationCode.second = property(element, "initializationCode").toString();
			mIsInterpretationalSemantics = property(element, "semanticsType").toString() == "Interpretation";
		}
	}
}

void VisualInterpreterUnit::initBeforeInterpretation()
{
	mCurrentNodesWithControlMark.clear();
	mInterpretersInterface.dehighlight();
	mMatches.clear();
	mRuleParser->clear();
	mRuleParser->setErrorReporter(mInterpretersInterface.errorReporter());
	resetRuleSyntaxCheck();
	mNeedToStopInterpretation = false;
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
	readInitialization();

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

		mRules.insert(ruleName, rule);
		foreach (Id const &ruleElement, ruleElements) {
			if (ruleElement.element() == "ControlFlowLocation" || ruleElement.element() == "Wildcard") {
				continue;
			}

			if (ruleElement.element() == "Replacement"){
				Id const fromId = fromInRule(ruleElement);
				Id const toId = toInRule(ruleElement);

				if (fromId == Id::rootId() || toId == Id::rootId()) {
					semanticsLoadingError(tr("Incorrect replacement in rule '") + ruleName + "'");
					return;
				}

				if (!mReplacedElements.contains(ruleName)) {
					mReplacedElements.insert(ruleName, new QHash<Id, Id>());
				}
				mReplacedElements.value(ruleName)->insert(fromId, toId);
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

				if (semanticsStatus.isEmpty() || semanticsStatus == "@deleted@") {
					putIdIntoMap(&mNodesWithControlMark, ruleName, nodeWithControl);
					if (semanticsStatus == "@deleted@") {
						putIdIntoMap(&mNodesWithDeletedControlMark, ruleName, nodeWithControl);
					}
				} else {
					putIdIntoMap(&mNodesWithNewControlMark, ruleName, nodeWithControl);
				}
				continue;
			}

			if (semanticsStatus == "@new@") {
				putIdIntoMap(&mCreatedElements, ruleName, ruleElement);
			} else if (semanticsStatus == "@deleted@") {
				putIdIntoMap(&mDeletedElements, ruleName, ruleElement);
			}
		}
	}

	orderRulesByPriority();

	mIsSemanticsLoaded = true;
	mInterpretersInterface.errorReporter()->clear();
	report(tr("Semantics loaded successfully"), false);
}

void VisualInterpreterUnit::interpret()
{
	mInterpretersInterface.errorReporter()->clear();

	if (!mIsSemanticsLoaded) {
		report(tr("Semantics not loaded"), true);
		return;
	}

	initBeforeInterpretation();
	interpretInitializationCode();

	int const timeout = mIsInterpretationalSemantics ?
			SettingsManager::value("debuggerTimeout").toInt() : SettingsManager::value("generationTimeout").toInt();

	while (findMatch()) {
		if (mNeedToStopInterpretation) {
			mPythonInterpreter->terminateProcess();
			report(tr("Interpretation stopped manually"), false);
			return;
		}

		if (hasRuleSyntaxError()) {
			report(tr("Rule '") + mMatchedRuleName
					+ tr("' cannot be applied because semantics has syntax errors"), true);
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
		mInterpretersInterface.dehighlight();
		mPythonInterpreter->deleteTempFile();
	}
	mPythonInterpreter->terminateProcess();
}

void VisualInterpreterUnit::stopInterpretation()
{
	mNeedToStopInterpretation = true;
}

void VisualInterpreterUnit::highlightMatch()
{
	foreach (Id const &id, mMatch.keys()) {
		mInterpretersInterface.highlight(mMatch.value(id), false);
	}

	pause(2000);

	mInterpretersInterface.dehighlight();
}

bool VisualInterpreterUnit::findMatch()
{
	foreach (QString const &ruleName, mOrderedRules) {
		mCurrentRuleName = ruleName;
		mRuleToFind = mRules.value(ruleName);
		if (checkRuleMatching() && checkApplicationCondition(ruleName)) {
			mMatchedRuleName = ruleName;
			return true;
		}
	}

	return false;
}

bool VisualInterpreterUnit::checkApplicationCondition(QString const &ruleName)
{
	if (!property(mRules.value(ruleName), "applicationCondition").toString().isEmpty()) {
		bool result = false;
		QList<QHash<Id, Id> > filteredMatches;
		for (int i = 0; i < mMatches.size(); i++) {
			if (checkApplicationCondition(mMatches.at(i), ruleName)) {
				result = true;
				filteredMatches.append(mMatches.at(i));
			}
		}
		mMatches = filteredMatches;
		return result;
	}
	return true;
}

bool VisualInterpreterUnit::checkApplicationCondition(QHash<Id, Id> const &match, QString const &ruleName) const
{
	QString const appCond = property(mRules.value(ruleName), "applicationCondition").toString();
	if (property(mRules.value(ruleName), "type").toString() == "Python") {
		return checkApplicationConditionPython(match, ruleName);
	} else {
		return checkApplicationConditionCStyle(match, appCond);
	}
}

bool VisualInterpreterUnit::checkApplicationConditionCStyle(QHash<Id, Id> const &match, QString const &appCond) const
{
	return mRuleParser->parseApplicationCondition(appCond, match);
}

bool VisualInterpreterUnit::checkApplicationConditionPython(QHash<Id, Id> const &match, QString const &ruleName) const
{
	QString const pythonPath = SettingsManager::value("pythonPath").toString();

	mPythonInterpreter->setPythonPath(pythonPath);

	mPythonGenerator->setRule(mRules.value(ruleName));
	mPythonGenerator->setMatch(match);

	return mPythonInterpreter->interpret(mPythonGenerator->generateScript(true), PythonInterpreter::applicationCondition);
}

Id VisualInterpreterUnit::startElement() const
{
	if (mNodesWithControlMark.contains(mCurrentRuleName)) {
		foreach (Id const &element, *mNodesWithControlMark.value(mCurrentRuleName)) {
			if (!hasProperty(element, "semanticsStatus") ||
					property(element, "semanticsStatus").toString() != "@new@")
			{
				return element;
			}
		}
	}

	IdList const elementsInRule = children(mRuleToFind);

	foreach (Id const &element, elementsInRule) {
		if (!isEdgeInRule(element) && element.element() != "ControlFlowMark") {
			if (!hasProperty(element, "semanticsStatus") ||
					property(element, "semanticsStatus").toString() != "@new@")
			{
				return element;
			}
		}
	}

	return Id::rootId();
}

void VisualInterpreterUnit::report(const QString &message, bool isError) const
{
	BaseGraphTransformationUnit::report(message, isError);
	if (isError) {
		mPythonInterpreter->terminateProcess();
	}
}

bool VisualInterpreterUnit::deleteElements()
{
	QHash<Id, Id> firstMatch = mMatches.at(0);

	if (mDeletedElements.contains(mMatchedRuleName)) {
		foreach (Id const &id, *(mDeletedElements.value(mMatchedRuleName))) {
			Id const node = firstMatch.value(id);
			mInterpretersInterface.dehighlight(node);
			mCurrentNodesWithControlMark.removeOne(node);

			mInterpretersInterface.deleteElementFromDiagram(
					mGraphicalModelApi.logicalId(firstMatch.value(id)));
		}
		return true;
	}
	return false;
}

bool VisualInterpreterUnit::createElements()
{
	QHash<Id, Id> *firstMatch = &mMatches.first();

	if (mCreatedElements.contains(mMatchedRuleName)) {
		mCreatedElementsPairs.clear();
		foreach (Id const &id, *(mCreatedElements.value(mMatchedRuleName))) {
			Id const createdId = Id(mInterpretersInterface.activeDiagram().editor()
					, mInterpretersInterface.activeDiagram().diagram()
					, id.element()
					, QUuid::createUuid().toString());
			Id const createdElem = mGraphicalModelApi.createElement(
					mInterpretersInterface.activeDiagram()
					, createdId
					, false
					, id.element()
					, position());

			mCreatedElementsPairs.insert(id, createdElem);
			firstMatch->insert(id, createdElem);
		}

		arrangeConnections();

		return true;
	}
	return false;
}

void VisualInterpreterUnit::arrangeConnections()
{
	QHash<Id, Id> firstMatch = mMatches.at(0);

	foreach (Id const &idInRule, mCreatedElementsPairs.keys()) {
		Id const idInModel = mCreatedElementsPairs.value(idInRule);

		Id const toInRul = toInRule(idInRule);
		if (toInRul != Id::rootId()) {
			mGraphicalModelApi.setTo(idInModel, firstMatch.value(toInRul));
		}

		Id const fromInRul = fromInRule(idInRule);
		if (fromInRul != Id::rootId()) {
			mGraphicalModelApi.setFrom(idInModel, firstMatch.value(fromInRul));
		}
	}
}

QPointF VisualInterpreterUnit::position()
{
	IdList const elements = elementsFromActiveDiagram();
	int x = 0;
	int y = 0;
	foreach (Id const &element, elements) {
		QPointF pos = mGraphicalModelApi.position(element);
		x = pos.x() > x ? pos.x() : x;
	}
	x += 50;
	return QPointF(x, y);
}

bool VisualInterpreterUnit::createElementsToReplace()
{
	QHash<Id, Id> *firstMatch = &mMatches.first();

	if (mReplacedElements.contains(mMatchedRuleName)) {
		mReplacedElementsPairs.clear();
		foreach (Id const &fromId, mReplacedElements.value(mMatchedRuleName)->keys()) {
			Id const toInRule = mReplacedElements.value(mMatchedRuleName)->value(fromId);
			Id const fromInModel = firstMatch->value(fromId);

			Id const toInModelId = Id(mInterpretersInterface.activeDiagram().editor()
					, mInterpretersInterface.activeDiagram().diagram()
					, toInRule.element()
					, QUuid::createUuid().toString());
			Id const toInModel = mGraphicalModelApi.createElement(
					mInterpretersInterface.activeDiagram()
					, toInModelId
					, false
					, toInRule.element()
					, mGraphicalModelApi.position(fromInModel));

			mReplacedElementsPairs.insert(fromInModel, toInModel);
			firstMatch->insert(toInRule, toInModel);

			copyProperties(mGraphicalModelApi.logicalId(toInModel), toInRule);
		}
		return true;
	}
	return false;
}

void VisualInterpreterUnit::replaceElements()
{
	if (mReplacedElements.contains(mMatchedRuleName)) {
		foreach (Id const &fromInModel, mReplacedElementsPairs.keys()) {
			Id const toInModel = mReplacedElementsPairs.value(fromInModel);

			foreach (Id const &link, outgoingLinks(fromInModel)) {
				mGraphicalModelApi.setFrom(link, toInModel);
			}
			foreach (Id const &link, incomingLinks(fromInModel)) {
				mGraphicalModelApi.setTo(link, toInModel);
			}

			mInterpretersInterface.deleteElementFromDiagram(
					mGraphicalModelApi.logicalId(fromInModel));
		}
	}
}

void VisualInterpreterUnit::moveControlFlow()
{
	QHash<Id, Id> firstMatch = mMatches.at(0);

	if (mNodesWithDeletedControlMark.contains(mMatchedRuleName)) {
		foreach (Id const &id, *(mNodesWithDeletedControlMark.value(mMatchedRuleName))) {
			Id const node = firstMatch.value(id);
			mInterpretersInterface.dehighlight(node);
			mCurrentNodesWithControlMark.removeOne(node);
		}
	}

	if (mNodesWithNewControlMark.contains(mMatchedRuleName)) {
		foreach (Id const &id, *(mNodesWithNewControlMark.value(mMatchedRuleName))) {
			Id const node = firstMatch.value(id);
			mInterpretersInterface.highlight(node, false);
			mCurrentNodesWithControlMark.prepend(node);
		}
	}
}

void VisualInterpreterUnit::interpretInitializationCode()
{
	if (mInitializationCode.first.isEmpty()) {
		return;
	}
	if (mInitializationCode.first == "Block Scheme (C-like)") {
		mRuleParser->parseStringCode(mInitializationCode.second);
	} else {
		mPythonInterpreter->interpret(mInitializationCode.second, PythonInterpreter::initialization);
	}
}

bool VisualInterpreterUnit::interpretReaction()
{
	QHash<Id, Id> const firstMatch = mMatches.at(0);

	Id const rule = mRules.value(mMatchedRuleName);
	QString const ruleProcess = property(rule, "procedure").toString();
	bool result = true;
	if (!ruleProcess.isEmpty()) {
		mRuleParser->setRuleId(rule);
		result = mRuleParser->parseRule(ruleProcess, firstMatch);
	}
	return result;
}

bool VisualInterpreterUnit::interpretPythonReaction()
{
	QString const pythonPath = SettingsManager::value("pythonPath").toString();

	mPythonInterpreter->setPythonPath(pythonPath);

	mPythonGenerator->setRule(mRules.value(mMatchedRuleName));
	mPythonGenerator->setMatch(mMatches.first());

	return mPythonInterpreter->interpret(mPythonGenerator->generateScript(false), PythonInterpreter::reaction);
}

void VisualInterpreterUnit::copyProperties(Id const &elemInModel, Id const &elemInRule)
{
	QHash<QString, QVariant> ruleProperties = properties(elemInRule);

	foreach (QString const &key, ruleProperties.keys()) {
		QVariant const value = ruleProperties.value(key);

		if (value.toString().isEmpty()) {
			continue;
		}

		setProperty(elemInModel, key, value);
	}
}

bool VisualInterpreterUnit::makeStep()
{
	bool needToUpdate = createElements();
	needToUpdate |= createElementsToReplace();

	bool result;
	if (property(mRules.value(mMatchedRuleName), "type").toString() == "Python") {
		result = interpretPythonReaction();
	} else {
		result = interpretReaction();
	}

	needToUpdate |= deleteElements();
	replaceElements();

	if (needToUpdate) {
		mInterpretersInterface.updateActiveDiagram();
	}

	moveControlFlow();

	mMatches.clear();
	return result;
}

bool VisualInterpreterUnit::compareElements(Id const &first, Id const &second) const
{
	bool result = true;

	if (mNodesWithControlMark.contains(mCurrentRuleName)) {
		if (mNodesWithControlMark.value(mCurrentRuleName)->contains(second)) {
			result = mCurrentNodesWithControlMark.contains(first);
		}
		if (mCurrentNodesWithControlMark.contains(first)) {
			result = mNodesWithControlMark.value(mCurrentRuleName)->contains(second);
		}
	} else if (mCurrentNodesWithControlMark.contains(first)) {
		return false;
	}

	return result && BaseGraphTransformationUnit::compareElements(first, second);
}

bool VisualInterpreterUnit::compareElementTypesAndProperties(Id const &first, Id const &second) const
{
	if (second.element() == "Wildcard") {
		return !isEdgeInModel(first);
	}

	return BaseGraphTransformationUnit::compareElementTypesAndProperties(first, second);
}

Id VisualInterpreterUnit::nodeIdWithControlMark(Id const &controlMarkId) const
{
	IdList const outLinks = outgoingLinks(controlMarkId);
	if (outLinks.size() == 0) {
		return Id::rootId();
	}

	return toInRule(outLinks.at(0));
}

IdList VisualInterpreterUnit::linksInRule(Id const &id) const
{
	IdList result;
	foreach (Id const &link, mLogicalModelApi.logicalRepoApi().links(id)) {
		if (link.element() != "Replacement" && link.element() != "ControlFlowLocation") {
			QString const semStatus = property(link, "semanticsStatus").toString();
			if (semStatus != "@new@") {
				result.append(link);
			}
		}
	}

	return result;
}

void VisualInterpreterUnit::semanticsLoadingError(QString const &message)
{
	report(message + tr(" Semantics loading failed."), true);
	mIsSemanticsLoaded = false;
}

utils::ExpressionsParser* VisualInterpreterUnit::ruleParser()
{
	return mRuleParser;
}

void VisualInterpreterUnit::processPythonInterpreterStdOutput(QHash<QPair<QString, QString>, QString> const &output)
{
	QPair<QString, QString> pair;
	foreach (pair, output.keys()) {
		QString const elemName = pair.first;
		QString const propName = pair.second;
		QString value = output.value(pair);
		if (value == "True" || value == "False") {
			value = value.toLower();
		}

		setProperty(mMatches.first().value(mPythonGenerator->idByName(elemName))
				, propName, QString::fromUtf8(value.toLatin1()));
	}

	mPythonInterpreter->continueStep();
}

void VisualInterpreterUnit::processPythonInterpreterErrOutput(QString const &output)
{
	mInterpretersInterface.errorReporter()->addCritical(output);
	mPythonInterpreter->continueStep();
}
