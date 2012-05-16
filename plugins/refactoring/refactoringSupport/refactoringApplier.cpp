#include "refactoringApplier.h"

#include <QEventLoop>
#include <QSet>
#include <QtCore/QDebug>

using namespace qReal;

QSet<QString> const defaultProperties = (QSet<QString>()
		<< "from" << "incomingConnections" << "incomingUsages" << "links"
		<< "name" << "outgoingConnections" << "outgoingUsages" << "to");

RefactoringApplier::RefactoringApplier(
		LogicalModelAssistInterface &logicalModelApi
		, GraphicalModelAssistInterface &graphicalModelApi
		, gui::MainWindowInterpretersInterface &interpretersInterface
		, qrRepo::RepoApi *refactoringRepoApi
		, QHash<Id, Id> *match)
		: mInterpretersInterface(interpretersInterface)
		, mLogicalModelApi(logicalModelApi)
		, mGraphicalModelApi(graphicalModelApi)
		, mRefactoringRepoApi(refactoringRepoApi)
		, mMatch(match)
{
}

RefactoringApplier::~RefactoringApplier()
{
}

IdList RefactoringApplier::getElementsFromBeforeBlock()
{
	return getElementsFromBlock("BeforeBlock");
}

IdList RefactoringApplier::getElementsFromAfterBlock()
{
	return getElementsFromBlock("AfterBlock");
}

IdList RefactoringApplier::getElementsFromBlock(QString const &blockType)
{
	IdList list;
	IdList resultList;
	IdList const refactoringElements = mRefactoringRepoApi->children(Id::rootId());
	foreach (Id const &refactoringElement, refactoringElements) {
		if (mRefactoringRepoApi->isGraphicalElement(refactoringElement)) {
			if (refactoringElement.element() == "RefactoringDiagramNode") {
				list = mRefactoringRepoApi->children(refactoringElement);
				foreach (Id const &id, list) {
					if (id.element() == blockType) {
						resultList.append(mRefactoringRepoApi->children(id));
						break;
					}
				}
				foreach (Id const &id, list) {
					if (id.element() == "Link" && resultList.contains(toInRule(id))
					&& resultList.contains(fromInRule(id)))
						resultList.append(id);
				}
			}
		}
	}
	return resultList;
}

Id RefactoringApplier::idElementWithID(QString const &IDValue, IdList const &idList)
{
	foreach (Id const &id, idList) {
		if (propertyID(id) == IDValue)
			return id;
	}
	return Id::rootId();
}

QString RefactoringApplier::propertyID(Id const &id)
{
	return mRefactoringRepoApi->property(mRefactoringRepoApi->logicalId(id), "ID").toString();
}

void RefactoringApplier::loadRefactoringRule()
{
	IdList const before = mMatch->keys();
	IdList const after = getElementsFromAfterBlock();

	mRule = new QList<QPair<Id, Id> >;
	mApply = new QList<QPair<Id, Id> >;

	mInterpretersInterface.dehighlight();

	foreach (Id const &beforeId, before) {
		QString const beforeElementID = getRefactoringProperty(mRefactoringRepoApi->logicalId(beforeId), "ID").toString();
		if (beforeElementID == "noThisProperty")
			continue;
		Id const elementAfterId = idElementWithID(beforeElementID, after);
		mRule->append(qMakePair(beforeId, elementAfterId));
		mApply->append(qMakePair(elementAfterId, mMatch->value(beforeId)));
	}
	foreach (Id const &afterId, after) {
		QString const afterElementID = getRefactoringProperty(mRefactoringRepoApi->logicalId(afterId), "ID").toString();
		Id const elementBeforeId = idElementWithID(afterElementID, before);
		if (elementBeforeId == Id::rootId()) {
			mRule->append(qMakePair(elementBeforeId, afterId));
			mApply->append(qMakePair(afterId, Id::rootId()));
		}
	}
}

bool RefactoringApplier::hasProperty(Id const &id, QString const &propertyName) const
{
	if (mRefactoringRepoApi->isLogicalElement(id)) {
		return mRefactoringRepoApi->hasProperty(id, propertyName);
	} else {
		return mRefactoringRepoApi->hasProperty(
				mRefactoringRepoApi->logicalId(id), propertyName);
	}
}

QVariant RefactoringApplier::getRefactoringProperty(Id const &id, QString const &propertyName) const
{
	if (mRefactoringRepoApi->hasProperty(id, propertyName))
		return mRefactoringRepoApi->property(id, propertyName);
	return "noThisProperty";
}

QVariant RefactoringApplier::getProperty(Id const &id, QString const &propertyName) const
{
	if (mRefactoringRepoApi->isLogicalElement(id)) {
		return mRefactoringRepoApi->property(id, propertyName);
	} else {
		return mRefactoringRepoApi->property(
				mRefactoringRepoApi->logicalId(id), propertyName);
	}
}

void RefactoringApplier::applyRefactoringRule()
{
	loadRefactoringRule();
	changeNamesRefactoring();
}

IdList RefactoringApplier::getApplyElementsTo()
{
	IdList result;
	for (int i = 0; i < mApply->size(); ++i) {
		result.append(mApply->at(i).second);
	}
	return result;
}

void RefactoringApplier::changeNamesRefactoring()
{
	for (int i = 0; i < mApply->size(); ++i) {
		QPair<Id, Id> const pair = mApply->at(i);
		changeElement(pair.second, pair.first);
	}
}

void RefactoringApplier::changeElement(Id const &changeFromId, Id const &changeToId)
{
	Id const beforeId = beforeIdInRule(changeToId);
	if (isElementTypesInRuleIdentical(beforeId, changeToId)) {
		if (changeToId.element() == "Element") {
			if (mRefactoringRepoApi->name(changeToId) == "(Element)")
				return;
			changeElementName(changeFromId, changeToId);
		}
		else if (changeToId.element() == "Link") {
			if (mRefactoringRepoApi->name(changeToId) != "(Link)")
				changeElementName(changeFromId, changeToId);
			checkDirection(changeFromId, changeToId, beforeId);
		}
		else {
			changePropertiesInModel(changeFromId, changeToId);
			if (!isNodeInRule(beforeId) && !isNodeInRule(changeToId))
				checkDirection(changeFromId, changeToId, beforeId);
		}
	}
	else
		changeElementInModel(changeFromId, changeToId);
}

Id RefactoringApplier::beforeIdInRule(Id const &id)
{
	for (int i = 0; i < mRule->size(); ++i) {
		QPair<Id, Id> pair = mRule->at(i);
		if (pair.second == id)
			return pair.first;
	}
	return Id::rootId();
}

bool RefactoringApplier::isElementTypesInRuleIdentical(Id const &beforeId, Id const &afterId)
{
	return (beforeId.element() == afterId.element() && beforeId.diagram() == afterId.diagram());
}

void RefactoringApplier::changePropertiesInModel(Id const &changeFromId, Id const &changeToId)
{
	changeElementName(changeFromId, changeToId);

	QHash<QString, QVariant> currentProperties = getProperties(changeFromId);

	foreach (QString const &key, currentProperties.keys()) {
		QVariant const value = currentProperties.value(key);

		if (hasProperty(changeToId, key)) {
			QVariant propertyValue = getProperty(changeToId, key);
			if (propertyValue.toString().contains("EXIST"))
				setProperty(changeFromId, key, propertyValue.toString().replace("EXIST", value.toString()));
			else
				setProperty(changeFromId, key, propertyValue);
			}
		}
}

void RefactoringApplier::changeElementInModel(const Id &changeFromId, const Id &changeToId)
{
	Q_UNUSED(changeFromId);
	Q_UNUSED(changeToId);

}

void RefactoringApplier::setProperty(Id const &id, QString const &propertyName
		, QVariant const &value) const
{
	if (mLogicalModelApi.isLogicalId(id)) {
		return mLogicalModelApi.logicalRepoApi().setProperty(id, propertyName, value);
	} else {
		return mLogicalModelApi.logicalRepoApi().setProperty(
				mGraphicalModelApi.logicalId(id), propertyName, value);
	}
}

QHash<QString, QVariant> RefactoringApplier::getProperties(Id const &id) const
{
	QHash<QString, QVariant> result;

	QMapIterator<QString, QVariant> properties =
			(mLogicalModelApi.isLogicalId(id))
			? mLogicalModelApi.logicalRepoApi().propertiesIterator(id)
			: mLogicalModelApi.logicalRepoApi().propertiesIterator(
					mGraphicalModelApi.logicalId(id));

	while (properties.hasNext()) {
		properties.next();

		if (!defaultProperties.contains(properties.key())) {
			result.insert(properties.key(), properties.value());
		}
	}

	return result;
}

void RefactoringApplier::changeElementName(const Id &changeFromId, const Id &changeToId)
{
	QString currentToName = mRefactoringRepoApi->name(changeToId);
	if (currentToName.contains("EXIST")) {
		QString const currentFromName = mGraphicalModelApi.name(changeFromId);
		QString const actualName = currentToName.replace("EXIST", currentFromName);
		mGraphicalModelApi.setName(changeFromId, actualName);
	}
	else {
		mGraphicalModelApi.setName(changeFromId, currentToName);
	}
}

bool RefactoringApplier::isNodeInModel(Id const &id)
{
	return (toInModel(id) == Id::rootId() && fromInModel(id) == Id::rootId());
}

bool RefactoringApplier::isNodeInRule(Id const &id)
{
	return (toInRule(id) == Id::rootId() && fromInRule(id) == Id::rootId());
}


Id RefactoringApplier::toInRule(Id const &id) const
{
	return mRefactoringRepoApi->to(id);
}

Id RefactoringApplier::fromInRule(Id const &id) const
{
	return mRefactoringRepoApi->from(id);
}

Id RefactoringApplier::toInModel(Id const &id) const
{
	return mLogicalModelApi.logicalRepoApi().to(id);
}

Id RefactoringApplier::fromInModel(Id const &id) const
{
	return mLogicalModelApi.logicalRepoApi().from(id);
}

void RefactoringApplier::checkDirection(Id const &changeFromId, Id const &changeToId, Id const &beforeId)
{
	if (propertyID(toInRule(beforeId)) == propertyID(toInRule(changeToId)))
		return;
	Id const oldTo = toInModel(changeFromId);
	Id const oldFrom = fromInModel(changeFromId);
	mGraphicalModelApi.setFrom(changeFromId, oldTo);
	mGraphicalModelApi.setTo(changeFromId, oldFrom);
}

Id RefactoringApplier::subprogramElementId()
{
	IdList result;
	IdList const after = getElementsFromAfterBlock();
	foreach (Id const &id, after) {
		if (mRefactoringRepoApi->isGraphicalElement(id)
				&& (isNodeInRule(id)))
			result.append(id);
	}
	if (result.size() == 1)
		return result.first();
	return Id::rootId();
}
