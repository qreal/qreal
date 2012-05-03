#include "refactoringFinder.h"

#include <QEventLoop>
#include <QSet>
#include <QtCore/QDebug>

using namespace qReal;

QSet<QString> const defaultProperties = (QSet<QString>()
		<< "from" << "incomingConnections" << "incomingUsages" << "links"
		<< "name" << "outgoingConnections" << "outgoingUsages" << "to"
/*		<< "ID"*/);

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
	mNodesWithNewControlMark = new IdList();
	mNodesWithDeletedControlMark = new IdList();
	mNodesWithControlMark = new IdList();

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
	foreach (Id const &id, mMatch->keys()) {
		mInterpretersInterface.highlight(mMatch->value(id), false);
//		qDebug() << id.toString() << mMatch->value(id).toString();
	}
	QEventLoop loop;
	QTimer::singleShot(2000, &loop, SLOT(quit()));
	loop.exec();

	mInterpretersInterface.dehighlight();
	}
	else {
		mInterpretersInterface.errorReporter()->addInformation("Not Found");
	}
}

bool RefactoringFinder::findMatch()
{
	return checkRuleMatching();
}

bool RefactoringFinder::checkRuleMatching()
{
	mMatch = new QHash<Id, Id>();

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
				return true;
			}
		}
	}

	return false;
}

bool RefactoringFinder::checkRuleMatchingRecursively()
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
			Id const linkEndInModel = getLinkEndModel(linkInModel, nodeInModel);
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
	Id const linkTo = to(linkInRule);
	if (linkTo == nodeInRule) {
		return from(linkInRule);
	}
	return linkTo;
}

Id RefactoringFinder::getProperLink(Id const &nodeInModel,
		 Id const &linkInRule, Id const &linkEndInRule) const
{
	IdList const linksInModel = links(nodeInModel);
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
	IdList const linksInModel = links(nodeInModel);
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
		if (!isEdge(beforeId))
			return beforeId;
	}
	return Id::rootId();
}

bool RefactoringFinder::compareLinks(Id const &first,Id const &second) const
{
	return compareElementTypesAndProperties(first, second)
			&& compareElements(mLogicalModelApi.logicalRepoApi().to(first), to(second))
			&& compareElements(mLogicalModelApi.logicalRepoApi().from(first), from(second));
}

bool RefactoringFinder::compareElements(Id const &first, Id const &second) const
{
	bool result = compareElementTypesAndProperties(first, second);

//	if (mNodesWithControlMark->contains(second)) {
//			result = result && mCurrentNodesWithControlMark.contains(first);
//		}
//		if (mCurrentNodesWithControlMark.contains(first)) {
//			result = result && mNodesWithControlMark->contains(second);
//		}

	return result;
}

bool RefactoringFinder::compareElementTypesAndProperties(Id const &first,
		Id const &second) const
{
	if (second.element() == "Element" || second.element() == "Link") {
		QString const elementName = mRefactoringRepoApi->name(second);
		if (elementName == "(Element)" || elementName == "(Link)" || elementName.contains("EXIST"))
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

bool RefactoringFinder::isEdge(Id const &element) const
{
	return to(element) != Id::rootId() ||
			from(element) != Id::rootId();
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

Id RefactoringFinder::to(Id const &id) const
{
	return mRefactoringRepoApi->to(id);
}

Id RefactoringFinder::from(Id const &id) const
{
	return mRefactoringRepoApi->from(id);
}

IdList RefactoringFinder::outgoingLinks(Id const &id) const
{
	return mLogicalModelApi.logicalRepoApi().outgoingLinks(id);
}

IdList RefactoringFinder::incomingLinks(Id const &id) const
{
	return mLogicalModelApi.logicalRepoApi().incomingLinks(id);
}

IdList RefactoringFinder::links(Id const &id) const
{
	return mLogicalModelApi.logicalRepoApi().links(id);
}

IdList RefactoringFinder::children(Id const &id) const
{
	return mGraphicalModelApi.graphicalRepoApi().children(id);
}

Id RefactoringFinder::getLinkEndModel(const qReal::Id &linkInModel, const qReal::Id &nodeInModel) const
{
	Id const linkTo = mLogicalModelApi.logicalRepoApi().to(linkInModel);
	if (linkTo == nodeInModel) {
		return mLogicalModelApi.logicalRepoApi().from(linkInModel);
	}
	return linkTo;
}
