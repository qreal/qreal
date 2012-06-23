#include "baseGraphTransformationUnit.h"

#include <QEventLoop>
#include <QSet>

using namespace qReal;

BaseGraphTransformationUnit::BaseGraphTransformationUnit(
		qReal::LogicalModelAssistInterface const &logicalModelApi
		, qReal::GraphicalModelAssistInterface const &graphicalModelApi
		, qReal::gui::MainWindowInterpretersInterface &interpretersInterface)
		: mInterpretersInterface(interpretersInterface)
		, mLogicalModelApi(logicalModelApi)
		, mGraphicalModelApi(graphicalModelApi)
{
	defaultProperties = (QSet<QString>()
				<< "from" << "incomingConnections" << "incomingUsages" << "links"
				<< "name" << "outgoingConnections" << "outgoingUsages" << "to");
}

BaseGraphTransformationUnit::~BaseGraphTransformationUnit()
{
}

IdList BaseGraphTransformationUnit::getElementsFromActiveDiagram() const
{
	Id const activeDiagram = mInterpretersInterface.activeDiagram();
	return children(activeDiagram);
}

bool BaseGraphTransformationUnit::checkRuleMatching()
{
	mMatch = new QHash<Id, Id>();
	bool isMatched = false;

	Id const startElement = getStartElement();
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
	Id const linkInRule = getOutsideLink(nodeInRule);

	if (linkInRule != Id::rootId()) {
		Id const linkEndInRule = getLinkEndInRule(linkInRule, nodeInRule);
		Id const nodeInModel = mMatch->value(nodeInRule);
		IdList const linksInModel = getProperLinks(nodeInModel, linkInRule);

		QHash<Id, Id> *matchBackup = mMatch;
		IdList *nodesHavingOutsideLinksBackup = &mNodesHavingOutsideLinks;
		IdList *currentMatchedGraphInRuleBackup = &mCurrentMatchedGraphInRule;
		int const posBackup = mPos;

		foreach (Id const &linkInModel, linksInModel) {
			Id const linkEndInModel = getLinkEndInModel(linkInModel, nodeInModel);
			if (checkNodeForAddingToMatch(linkEndInModel, linkEndInRule)) {
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
		Id const linkEndInRule = getLinkEndInRule(linkInRule, nodeInRule);
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

void BaseGraphTransformationUnit::rollback()
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

IdList BaseGraphTransformationUnit::getLinksToMatchedSubgraph(Id const &nodeInRule) const
{
	IdList result;
	IdList const lnksInRule = linksInRule(nodeInRule);
	foreach (Id const &link, lnksInRule) {
		Id const linkEnd = getLinkEndInRule(link, nodeInRule);
		if (mCurrentMatchedGraphInRule.contains(linkEnd)) {
			result.append(link);
		}
	}
	return result;
}

Id BaseGraphTransformationUnit::getOutsideLink(Id const &nodeInRule) const
{
	IdList const lnksInRule = linksInRule(nodeInRule);
	foreach (Id const &linkInRule, lnksInRule) {
		Id const linkEndInRule = getLinkEndInRule(linkInRule, nodeInRule);
		if (!mCurrentMatchedGraphInRule.contains(linkEndInRule)) {
			return linkInRule;
		}
	}

	return Id::rootId();
}

Id BaseGraphTransformationUnit::getLinkEndInModel(Id const &linkInModel,
		Id const &nodeInModel) const
{
	Id const linkTo = toInModel(linkInModel);
	if (linkTo == nodeInModel) {
		return fromInModel(linkInModel);
	}
	return linkTo;
}

Id BaseGraphTransformationUnit::getLinkEndInRule(Id const &linkInRule,
		Id const &nodeInRule) const
{
	Id const linkTo = toInRule(linkInRule);
	if (linkTo == nodeInRule) {
		return fromInRule(linkInRule);
	}
	return linkTo;
}

Id BaseGraphTransformationUnit::getProperLink(Id const &nodeInModel,
		 Id const &linkInRule, Id const &linkEndInRule) const
{
	IdList const lnksInModel = linksInModel(nodeInModel);
	foreach (Id const &linkInModel, lnksInModel) {
		if (compareLinks(linkInModel, linkInRule)) {
			Id const linkEndInModel = getLinkEndInModel(linkInModel, nodeInModel);
			if (linkEndInModel == mMatch->value(linkEndInRule)) {
				return linkInModel;
			}
		}
	}

	return Id::rootId();
}

IdList BaseGraphTransformationUnit::getProperLinks(Id const &nodeInModel,
		Id const &linkInRule) const
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

	return compareElementTypesAndProperties(first, second)
			&& compareElements(idTo1, idTo2)
			&& compareElements(idFrom1, idFrom2);
}

bool BaseGraphTransformationUnit::compareElements(Id const &first, Id const &second) const
{
	return compareElementTypesAndProperties(first, second);
}

bool BaseGraphTransformationUnit::compareElementTypesAndProperties(Id const &first,
		Id const &second) const
{
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

bool BaseGraphTransformationUnit::hasProperty(Id const &id, QString const &propertyName) const
{
	if (mLogicalModelApi.isLogicalId(id)) {
		return mLogicalModelApi.logicalRepoApi().hasProperty(id, propertyName);
	} else {
		return mLogicalModelApi.logicalRepoApi().hasProperty(
				mGraphicalModelApi.logicalId(id), propertyName);
	}
}

QHash<QString, QVariant> BaseGraphTransformationUnit::getProperties(Id const &id) const
{
	QHash<QString, QVariant> res;

	QMapIterator<QString, QVariant> properties = getPropertiesIterator(id);

	while (properties.hasNext()) {
		properties.next();

		if (!defaultProperties.contains(properties.key())) {
			res.insert(properties.key(), properties.value());
		}
	}

	return res;
}

QMapIterator<QString, QVariant> BaseGraphTransformationUnit::getPropertiesIterator(Id const &id) const
{
	return (mLogicalModelApi.isLogicalId(id))
			? mLogicalModelApi.logicalRepoApi().propertiesIterator(id)
			: mLogicalModelApi.logicalRepoApi().propertiesIterator(
					mGraphicalModelApi.logicalId(id));
}

QVariant BaseGraphTransformationUnit::getProperty(Id const &id, QString const &propertyName) const
{
	if (mLogicalModelApi.isLogicalId(id)) {
		return mLogicalModelApi.logicalRepoApi().property(id, propertyName);
	} else {
		return mLogicalModelApi.logicalRepoApi().property(
				mGraphicalModelApi.logicalId(id), propertyName);
	}
}

void BaseGraphTransformationUnit::setProperty(Id const &id, QString const &propertyName
		, QVariant const &value) const
{
	if (mLogicalModelApi.isLogicalId(id)) {
		return mLogicalModelApi.logicalRepoApi().setProperty(id, propertyName, value);
	} else {
		return mLogicalModelApi.logicalRepoApi().setProperty(
				mGraphicalModelApi.logicalId(id), propertyName, value);
	}
}

bool BaseGraphTransformationUnit::isEdgeInModel(Id const &element) const
{
	return toInModel(element) != Id::rootId() ||
			fromInModel(element) != Id::rootId();
}

bool BaseGraphTransformationUnit::isEdgeInRule(Id const &element) const
{
	return toInRule(element) != Id::rootId() ||
			fromInRule(element) != Id::rootId();
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

void BaseGraphTransformationUnit::report(QString const &message) const
{
	mInterpretersInterface.errorReporter()->addInformation(message);
}

QList<QHash<Id, Id> > BaseGraphTransformationUnit::getMatches()
{
	return mMatches;
}

void BaseGraphTransformationUnit::pause(int const &time)
{
	QEventLoop loop;
	QTimer::singleShot(time, &loop, SLOT(quit()));
	loop.exec();
}
