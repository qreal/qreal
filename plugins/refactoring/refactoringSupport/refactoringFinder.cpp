#include "refactoringFinder.h"

#include <QEventLoop>
#include <QSet>
#include <QtCore/QDebug>

using namespace qReal;

QSet<QString> const defaultProperties = (QSet<QString>()
		<< "from" << "incomingConnections" << "incomingUsages" << "links"
		<< "name" << "outgoingConnections" << "outgoingUsages" << "to");

RefactoringFinder::RefactoringFinder(
		const LogicalModelAssistInterface &logicalModelApi
		, const GraphicalModelAssistInterface &graphicalModelApi
		, gui::MainWindowInterpretersInterface &interpretersInterface
		, qrRepo::RepoApi *refactoringRepoApi)
		: mInterpretersInterface(interpretersInterface)
		, mLogicalModelApi(logicalModelApi)
		, mGraphicalModelApi(graphicalModelApi)
		, mRefactoringRepoApi(refactoringRepoApi)
{
}

RefactoringFinder::~RefactoringFinder()
{
}

IdList RefactoringFinder::getElementsFromBeforeBlock() const
{
	return getElementsFromBlock("BeforeBlock");
}

IdList RefactoringFinder::getElementsFromAfterBlock() const
{
	return getElementsFromBlock("AfterBlock");
}

IdList RefactoringFinder::getElementsFromBlock(QString const &blockType) const
{
	IdList list;
	IdList const refactoringElements = mRefactoringRepoApi->children(Id::rootId());
	foreach (Id const &refactoringElement, refactoringElements) {
		if (mRefactoringRepoApi->isGraphicalElement(refactoringElement)) {
			if (refactoringElement.element() == "RefactoringDiagramNode") {
				list = mRefactoringRepoApi->children(refactoringElement);
				foreach (Id const &id, list) {
					if (id.element() == blockType)
						return mRefactoringRepoApi->children(id);
				}
			}
		}
	}
	return IdList();
}

IdList RefactoringFinder::getElementsFromActiveDiagram() const
{
	Id const activeDiagram = mInterpretersInterface.activeDiagram();
	return children(activeDiagram);
}

bool RefactoringFinder::containElementWithID(QString const &IDValue, IdList const &idList)
{
	foreach (Id const &id, idList) {
		if (mRefactoringRepoApi->property(id, "ID").toString() == IDValue)
			return true;
	}
	return false;
}

void RefactoringFinder::addElement(Id const &id, IdList *idList)
{
	if (!idList->contains(id))
		idList->append(id);
}

void RefactoringFinder::loadRefactoringRule()
{
	IdList const before = getElementsFromBeforeBlock();
	IdList const after = getElementsFromAfterBlock();

	mDeletedElements = new IdList();
	mReplacedElements = new IdList();
	mCreatedElements = new IdList();

	mInterpretersInterface.dehighlight();

	foreach (Id const &beforeId, before) {
		QString const beforeElementID = getRefactoringProperty(beforeId, "ID").toString();
		if (containElementWithID(beforeElementID, after))
			addElement(beforeId, mReplacedElements);
		else
			addElement(beforeId, mDeletedElements);
	}
	foreach (Id const &afterId, after) {
		QString const afterElementID = getRefactoringProperty(afterId, "ID").toString();
		if (!containElementWithID(afterElementID, before))
			addElement(afterId, mCreatedElements);
	}
}


void RefactoringFinder::highlightMatch()
{
	if (findMatch()) {
		for (int i = 0; i < mMatches.size(); ++i) {
			QHash <Id, Id> currentMatch = mMatches.at(i);
			foreach (Id const &id, currentMatch.keys()) {
				QColor const color = QColor(SettingsManager::value("refactoringColor"
						, "cyan").toString());
				mInterpretersInterface.highlight(currentMatch.value(id), false, color);
				pause(500);
			}
			pause(500);
			mInterpretersInterface.dehighlight();
			pause(1000);
		}
	}
	else {
		mInterpretersInterface.errorReporter()->addInformation("Not Found");
	}
}

bool RefactoringFinder::findMatch()
{
	mMatches.clear();
	return checkRuleMatching();
}

bool RefactoringFinder::checkRuleMatching()
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

bool RefactoringFinder::checkRuleMatchingRecursively()
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
		Id const linkEndInRule = getLinkEnd(linkInRule, nodeInRule);
		Id const nodeInModel = mMatch->value(nodeInRule);
		IdList const linksInModel = getProperLinks(nodeInModel, linkInRule);

		QHash<Id, Id> *matchBackup = mMatch;
		IdList *nodesHavingOutsideLinksBackup = &mNodesHavingOutsideLinks;
		IdList *currentMatchedGraphInRuleBackup = &mCurrentMatchedGraphInRule;
		int const posBackup = mPos;

		foreach (Id const &linkInModel, linksInModel) {
			Id const linkEndInModel = getLinkEndModel(linkInModel, nodeInModel);
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

bool RefactoringFinder::checkNodeForAddingToMatch(Id const &nodeInModel,
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

bool RefactoringFinder::checkExistingLinks(Id const &nodeInModel,
		Id const &nodeInRule, QHash<Id, Id> *linksToAddInMatch)
{
	IdList const linksInRule = mRefactoringRepoApi->links(nodeInRule);

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

void RefactoringFinder::rollback()
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

IdList RefactoringFinder::getLinksToMatchedSubgraph(Id const &nodeInRule) const
{
	IdList result;
	IdList const linksInRule = mRefactoringRepoApi->links(nodeInRule);
	foreach (Id const &link, linksInRule) {
		Id const linkEnd = getLinkEnd(link, nodeInRule);
		if (mCurrentMatchedGraphInRule.contains(linkEnd)) {
			result.append(link);
		}
	}
	return result;
}

Id RefactoringFinder::getOutsideLink(Id const &nodeInRule) const
{
	IdList const linksInRule = mRefactoringRepoApi->links(nodeInRule);
	foreach (Id const &linkInRule, linksInRule) {
		Id const linkEndInRule = getLinkEnd(linkInRule, nodeInRule);
		if (!mCurrentMatchedGraphInRule.contains(linkEndInRule)) {
			return linkInRule;
		}
	}

	return Id::rootId();
}

Id RefactoringFinder::getLinkEnd(Id const &linkInRule, Id const &nodeInRule) const
{
	Id const linkTo = toInRule(linkInRule);
	if (linkTo == nodeInRule) {
		return fromInRule(linkInRule);
	}
	return linkTo;
}

Id RefactoringFinder::getProperLink(Id const &nodeInModel,
		 Id const &linkInRule, Id const &linkEndInRule) const
{
	IdList const linksInModel = linksModel(nodeInModel);
	foreach (Id const &linkInModel, linksInModel) {
		if (compareLinks(linkInModel, linkInRule)) {
			Id const linkEndInModel = getLinkEndModel(linkInModel, nodeInModel);
			if (linkEndInModel == mMatch->value(linkEndInRule)) {
				return linkInModel;
			}
		}
	}

	return Id::rootId();
}

IdList RefactoringFinder::getProperLinks(Id const &nodeInModel,
		Id const &linkInRule) const
{
	IdList result;
	IdList const linksInModel = linksModel(nodeInModel);
	foreach (Id const &linkInModel, linksInModel) {
		if (compareLinks(linkInModel, linkInRule)) {
			result.append(linkInModel);
		}
	}
	return result;
}

Id RefactoringFinder::getStartElement() const
{
	IdList const before = getElementsFromBeforeBlock();

	foreach (Id const &beforeId, before) {
		if (!isEdgeInRule(beforeId))
			return beforeId;
	}
	return Id::rootId();
}

bool RefactoringFinder::compareLinks(Id const &first,Id const &second) const
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

bool RefactoringFinder::compareElements(Id const &first, Id const &second) const
{
	return compareElementTypesAndProperties(first, second);
}

bool RefactoringFinder::compareElementTypesAndProperties(Id const &first,
		Id const &second) const
{
	bool firstIsNode = !isEdgeInModel(first);
	if (second.element() == "Element" && firstIsNode) {
		QString const elementName = mRefactoringRepoApi->name(second);
		if (elementName == "(Element)" || elementName.contains("EXIST"))
			return true;
		return (elementName == mGraphicalModelApi.name(first));
	}

	if (second.element() == "Link" && !firstIsNode) {
		QString const elementName = mRefactoringRepoApi->name(second);
		if (elementName == "(Link)" || elementName.contains("EXIST"))
			return true;
		return (elementName == mGraphicalModelApi.name(first));
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

bool RefactoringFinder::hasProperty(Id const &id, QString const &propertyName) const
{
	if (mLogicalModelApi. isLogicalId(id)) {
		return mLogicalModelApi.logicalRepoApi().hasProperty(id, propertyName);
	} else {
		return mLogicalModelApi.logicalRepoApi().hasProperty(
				mGraphicalModelApi.logicalId(id), propertyName);
	}
}

QHash<QString, QVariant> RefactoringFinder::getProperties(Id const &id) const
{
	QHash<QString, QVariant> res;

	QMapIterator<QString, QVariant> properties =
			mRefactoringRepoApi->propertiesIterator(id);

	while (properties.hasNext()) {
		properties.next();

		if (!defaultProperties.contains(properties.key())) {
			res.insert(properties.key(), properties.value());
		}
	}

	return res;
}

bool RefactoringFinder::isEdgeInRule(Id const &element) const
{
	return toInRule(element) != Id::rootId() ||
			fromInRule(element) != Id::rootId();
}

bool RefactoringFinder::isEdgeInModel(Id const &element) const
{
	return toInModel(element) != Id::rootId() ||
			fromInModel(element) != Id::rootId();
}

QVariant RefactoringFinder::getProperty(Id const &id, QString const &propertyName) const
{
	if (mLogicalModelApi.isLogicalId(id)) {
		return mLogicalModelApi.logicalRepoApi().property(id, propertyName);
	} else {
		return mLogicalModelApi.logicalRepoApi().property(
				mGraphicalModelApi.logicalId(id), propertyName);
	}
}

QVariant RefactoringFinder::getRefactoringProperty(Id const &id, QString const &propertyName) const
{
	return mRefactoringRepoApi->property(id, propertyName);
}

Id RefactoringFinder::toInRule(Id const &id) const
{
	return mRefactoringRepoApi->to(id);
}

Id RefactoringFinder::fromInRule(Id const &id) const
{
	return mRefactoringRepoApi->from(id);
}

Id RefactoringFinder::toInModel(Id const &id) const
{
	return mLogicalModelApi.logicalRepoApi().to(id);
}

Id RefactoringFinder::fromInModel(Id const &id) const
{
	return mLogicalModelApi.logicalRepoApi().from(id);
}

IdList RefactoringFinder::linksModel(Id const &id) const
{
	return mLogicalModelApi.logicalRepoApi().links(id);
}

IdList RefactoringFinder::children(Id const &id) const
{
	return mGraphicalModelApi.graphicalRepoApi().children(id);
}

Id RefactoringFinder::getLinkEndModel(const qReal::Id &linkInModel, const qReal::Id &nodeInModel) const
{
	Id const linkTo = toInModel(linkInModel);
	if (linkTo == nodeInModel) {
		return fromInModel(linkInModel);
	}
	return linkTo;
}

void RefactoringFinder::pause(const int &time)
{
	QEventLoop loop;
	QTimer::singleShot(time, &loop, SLOT(quit()));
	loop.exec();

}

QList<QHash<Id, Id> > RefactoringFinder::getMatches()
{
	return mMatches;
}
