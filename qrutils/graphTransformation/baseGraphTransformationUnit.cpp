#include "baseGraphTransformationUnit.h"

using namespace qReal;

BaseGraphTransformationUnit::BaseGraphTransformationUnit(
		qReal::LogicalModelAssistInterface &logicalModelApi
		, qReal::GraphicalModelAssistInterface &graphicalModelApi
		, qReal::gui::MainWindowInterpretersInterface &interpretersInterface)
		: mInterpretersInterface(interpretersInterface)
		, mLogicalModelApi(logicalModelApi)
		, mGraphicalModelApi(graphicalModelApi)
		, mHasRuleSyntaxErr(false)
{
	mDefaultProperties = (QSet<QString>()
		<< "from" << "incomingConnections" << "incomingUsages" << "links"
		<< "name" << "outgoingConnections" << "outgoingUsages" << "to");
}

BaseGraphTransformationUnit::~BaseGraphTransformationUnit()
{
}

IdList BaseGraphTransformationUnit::elementsFromActiveDiagram() const
{
	Id const activeDiagram = mInterpretersInterface.activeDiagram();
	return children(activeDiagram);
}

bool BaseGraphTransformationUnit::checkRuleMatching()
{
	mMatch = new QHash<Id, Id>();
	bool isMatched = false;

	Id const startElem = startElement();
	if (startElem == Id::rootId()) {
		report(tr("Rule '") + property(mRuleToFind, "ruleName").toString() +
				tr("' has not any appropriate nodes"), true);
		mHasRuleSyntaxErr = true;
		return false;
	}

	mNodesHavingOutsideLinks.append(startElem);

	IdList const elements = elementsFromActiveDiagram();
	foreach (Id const &element, elements) {
		if (compareElements(element, startElem)) {
			mCurrentMatchedGraphInRule.clear();
			mNodesHavingOutsideLinks.clear();
			mMatch->clear();
			mPos = 0;

			mCurrentMatchedGraphInRule.append(startElem);
			mNodesHavingOutsideLinks.append(startElem);
			mMatch->insert(startElem, element);

			if (checkRuleMatchingRecursively()) {
				isMatched = true;
			}
		}
	}

	return isMatched;
}

bool BaseGraphTransformationUnit::checkRuleMatchingRecursively()
{
	if (mNodesHavingOutsideLinks.length() == mPos) {
		QHash<Id, Id> currentMatch = *(mMatch);
		mMatches.append(currentMatch);
		return true;
	}

	bool isMatched = false;

	Id const nodeInRule = mNodesHavingOutsideLinks.at(mPos);
	Id const linkInRule = outsideLink(nodeInRule);

	if (linkInRule != Id::rootId()) {
		Id const linkEndInR = linkEndInRule(linkInRule, nodeInRule);
		if (linkEndInR == Id::rootId()) {
			report(tr("Rule '") + property(mRuleToFind, "ruleName").toString() +
					tr("' has unconnected link"), true);
			mHasRuleSyntaxErr = true;
			return false;
		}

		Id const nodeInModel = mMatch->value(nodeInRule);
		IdList const lnksInModel = properLinks(nodeInModel, linkInRule);

		QHash<Id, Id> *matchBackup = mMatch;
		IdList *nodesHavingOutsideLinksBackup = &mNodesHavingOutsideLinks;
		IdList *currentMatchedGraphInRuleBackup = &mCurrentMatchedGraphInRule;
		int const posBackup = mPos;

		foreach (Id const &linkInModel, lnksInModel) {
			Id const linkEndInM = linkEndInModel(linkInModel, nodeInModel);
			if (checkNodeForAddingToMatch(linkEndInM, linkEndInR)) {
				if (checkRuleMatchingRecursively()) {
					isMatched = true;
					mMatch = new QHash<Id, Id>();
					foreach (Id const id, matchBackup->keys()) {
						mMatch->insert(id, matchBackup->value(id));
					}

					mNodesHavingOutsideLinks = *(new IdList());
					foreach (Id const id, *nodesHavingOutsideLinksBackup) {
						mNodesHavingOutsideLinks.append(id);
					}

					mCurrentMatchedGraphInRule = *(new IdList());
					foreach (Id const id, *currentMatchedGraphInRuleBackup) {
						mCurrentMatchedGraphInRule.append(id);
					}
					mPos = posBackup;
				} else {
					rollback();
				}
			}
		}
		return isMatched;
	} else {
		mPos++;
		return checkRuleMatchingRecursively();
	}
}

bool BaseGraphTransformationUnit::checkNodeForAddingToMatch(Id const &nodeInModel,
		Id const &nodeInRule)
{
	if (nodeInModel == Id::rootId()) {
		return false;
	}

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

bool BaseGraphTransformationUnit::checkExistingLinks(Id const &nodeInModel,
		Id const &nodeInRule, QHash<Id, Id> *linksToAddInMatch)
{
	IdList const lnksInRule = linksInRule(nodeInRule);

	foreach (Id const &linkInRule, lnksInRule) {
		Id const linkEndInR = linkEndInRule(linkInRule, nodeInRule);
		if (mCurrentMatchedGraphInRule.contains(linkEndInR)) {
			Id const properLinkInModel = properLink(nodeInModel, linkInRule, linkEndInR);
			if (properLinkInModel == Id::rootId()) {
				return false;
			} else {
				linksToAddInMatch->insert(linkInRule, properLinkInModel);
			}
		}
	}

	return true;
}

void BaseGraphTransformationUnit::rollback()
{
	Id const nodeToRemove = mCurrentMatchedGraphInRule.last();

	mMatch->remove(nodeToRemove);
	mCurrentMatchedGraphInRule.removeLast();
	mNodesHavingOutsideLinks.removeLast();
	if (mPos == mNodesHavingOutsideLinks.length()) {
		mPos--;
	}

	IdList const links = linksToMatchedSubgraph(nodeToRemove);
	foreach (Id const &link, links) {
		mMatch->remove(link);
	}
}

IdList BaseGraphTransformationUnit::linksToMatchedSubgraph(Id const &nodeInRule) const
{
	IdList result;
	IdList const lnksInRule = linksInRule(nodeInRule);
	foreach (Id const &link, lnksInRule) {
		Id const linkEnd = linkEndInRule(link, nodeInRule);
		if (mCurrentMatchedGraphInRule.contains(linkEnd)) {
			result.append(link);
		}
	}
	return result;
}

Id BaseGraphTransformationUnit::outsideLink(Id const &nodeInRule) const
{
	IdList const lnksInRule = linksInRule(nodeInRule);
	foreach (Id const &linkInRule, lnksInRule) {
		Id const linkEndInR = linkEndInRule(linkInRule, nodeInRule);
		if (!mCurrentMatchedGraphInRule.contains(linkEndInR)) {
			return linkInRule;
		}
	}

	return Id::rootId();
}

Id BaseGraphTransformationUnit::linkEndInModel(Id const &linkInModel, Id const &nodeInModel) const
{
	Id const linkTo = toInModel(linkInModel);
	if (linkTo == nodeInModel) {
		return fromInModel(linkInModel);
	}
	return linkTo;
}

Id BaseGraphTransformationUnit::linkEndInRule(Id const &linkInRule, Id const &nodeInRule) const
{
	Id const linkTo = toInRule(linkInRule);
	if (linkTo == nodeInRule) {
		return fromInRule(linkInRule);
	}
	return linkTo;
}

Id BaseGraphTransformationUnit::properLink(Id const &nodeInModel, Id const &linkInRule, Id const &linkEndInR) const
{
	IdList const lnksInModel = linksInModel(nodeInModel);
	foreach (Id const &linkInModel, lnksInModel) {
		if (compareLinks(linkInModel, linkInRule)) {
			Id const linkEndInM = linkEndInModel(linkInModel, nodeInModel);
			if (linkEndInM == mMatch->value(linkEndInR)) {
				return linkInModel;
			}
		}
	}

	return Id::rootId();
}

IdList BaseGraphTransformationUnit::properLinks(Id const &nodeInModel, Id const &linkInRule) const
{
	IdList result;
	IdList const lnksInModel = linksInModel(nodeInModel);
	foreach (Id const &linkInModel, lnksInModel) {
		if (compareLinks(linkInModel, linkInRule)) {
			result.append(linkInModel);
		}
	}
	return result;
}

bool BaseGraphTransformationUnit::compareLinks(Id const &first,Id const &second) const
{
	Id const idTo1 = toInModel(first);
	Id const idTo2 = toInRule(second);
	Id const idFrom1 = fromInModel(first);
	Id const idFrom2 = fromInRule(second);

	bool result = compareElementTypesAndProperties(first, second)
			&& compareElements(idTo1, idTo2)
			&& compareElements(idFrom1, idFrom2);

	if (mMatch->contains(idTo2)) {
		result = result && mMatch->value(idTo2) == idTo1;
	}

	if (mMatch->contains(idFrom2)) {
		result = result && mMatch->value(idFrom2) == idFrom1;
	}

	return result;
}

bool BaseGraphTransformationUnit::compareElements(Id const &first, Id const &second) const
{
	return compareElementTypesAndProperties(first, second);
}

bool BaseGraphTransformationUnit::compareElementTypesAndProperties(Id const &first
		, Id const &second) const
{
	if (first.element() == second.element() && first.diagram() == second.diagram()) {
		QHash<QString, QVariant> secondProperties = properties(second);

		foreach (QString const &key, secondProperties.keys()) {
			QVariant const value = secondProperties.value(key);

			if (value.toString().isEmpty()) {
				continue;
			}

			if (!hasProperty(first, key) || property(first, key) != value) {
				return false;
			}
		}

		return true;
	}

	return false;
}

bool BaseGraphTransformationUnit::hasProperty(Id const &id, QString const &propertyName) const
{
	if (mLogicalModelApi.isLogicalId(id)) {
		return mLogicalModelApi.logicalRepoApi().hasProperty(id, propertyName);
	} else {
		return mLogicalModelApi.logicalRepoApi().hasProperty(
				mGraphicalModelApi.logicalId(id), propertyName);
	}
}

QHash<QString, QVariant> BaseGraphTransformationUnit::properties(Id const &id) const
{
	QHash<QString, QVariant> res;

	if (id != Id::rootId()) {
		QMapIterator<QString, QVariant> properties = propertiesIterator(id);

		while (properties.hasNext()) {
			properties.next();

			if (!mDefaultProperties.contains(properties.key())) {
				res.insert(properties.key(), properties.value());
			}
		}
	}

	return res;
}

QMapIterator<QString, QVariant> BaseGraphTransformationUnit::propertiesIterator(Id const &id) const
{
	return (mLogicalModelApi.isLogicalId(id))
			? mLogicalModelApi.logicalRepoApi().propertiesIterator(id)
			: mLogicalModelApi.logicalRepoApi().propertiesIterator(mGraphicalModelApi.logicalId(id));
}

QVariant BaseGraphTransformationUnit::property(Id const &id, QString const &propertyName) const
{
	if (mLogicalModelApi.isLogicalId(id)) {
		return mLogicalModelApi.logicalRepoApi().property(id, propertyName);
	}
	return mLogicalModelApi.logicalRepoApi().property(mGraphicalModelApi.logicalId(id), propertyName);
}

void BaseGraphTransformationUnit::setProperty(Id const &id, QString const &propertyName
		, QVariant const &value) const
{
	if (mLogicalModelApi.isLogicalId(id)) {
		mLogicalModelApi.mutableLogicalRepoApi().setProperty(id, propertyName, value);
	}
	mLogicalModelApi.mutableLogicalRepoApi().setProperty(
				mGraphicalModelApi.logicalId(id), propertyName, value);
}

bool BaseGraphTransformationUnit::isEdgeInModel(Id const &element) const
{
	return toInModel(element) != Id::rootId() || fromInModel(element) != Id::rootId();
}

bool BaseGraphTransformationUnit::isEdgeInRule(Id const &element) const
{
	return toInRule(element) != Id::rootId() || fromInRule(element) != Id::rootId();
}

Id BaseGraphTransformationUnit::toInModel(Id const &id) const
{
	return mLogicalModelApi.logicalRepoApi().to(id);
}

Id BaseGraphTransformationUnit::fromInModel(Id const &id) const
{
	return mLogicalModelApi.logicalRepoApi().from(id);
}

Id BaseGraphTransformationUnit::toInRule(Id const &id) const
{
	return mLogicalModelApi.logicalRepoApi().to(id);
}

Id BaseGraphTransformationUnit::fromInRule(Id const &id) const
{
	return mLogicalModelApi.logicalRepoApi().from(id);
}

IdList BaseGraphTransformationUnit::linksInModel(Id const &id) const
{
	return mLogicalModelApi.logicalRepoApi().links(id);
}

IdList BaseGraphTransformationUnit::linksInRule(Id const &id) const
{
	return mLogicalModelApi.logicalRepoApi().links(id);
}

IdList BaseGraphTransformationUnit::outgoingLinks(Id const &id) const
{
	return mLogicalModelApi.logicalRepoApi().outgoingLinks(id);
}

IdList BaseGraphTransformationUnit::incomingLinks(Id const &id) const
{
	return mLogicalModelApi.logicalRepoApi().incomingLinks(id);
}

IdList BaseGraphTransformationUnit::children(Id const &id) const
{
	return mGraphicalModelApi.graphicalRepoApi().children(id);
}

void BaseGraphTransformationUnit::report(QString const &message, bool isError) const
{
	if (isError) {
		mInterpretersInterface.errorReporter()->addCritical(message);
	} else {
		mInterpretersInterface.errorReporter()->addInformation(message);
	}
}

QList<QHash<Id, Id> > BaseGraphTransformationUnit::matches()
{
	return mMatches;
}

void BaseGraphTransformationUnit::pause(int const &time)
{
	QEventLoop loop;
	QTimer::singleShot(time, &loop, SLOT(quit()));
	loop.exec();
}

bool BaseGraphTransformationUnit::hasRuleSyntaxError()
{
	return mHasRuleSyntaxErr;
}

void BaseGraphTransformationUnit::resetRuleSyntaxCheck()
{
	mHasRuleSyntaxErr = false;
}
