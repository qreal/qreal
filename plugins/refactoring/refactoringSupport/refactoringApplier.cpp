#include "refactoringApplier.h"

#include <QEventLoop>
#include <QSet>
#include <QtCore/QDebug>

using namespace qReal;

QSet<QString> const defaultProperties = (QSet<QString>()
		<< "from" << "incomingConnections" << "incomingUsages" << "links"
		<< "name" << "outgoingConnections" << "outgoingUsages" << "to");

RefactoringApplier::RefactoringApplier(
		const LogicalModelAssistInterface &logicalModelApi
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

IdList RefactoringApplier::getElementsFromBeforeBlock() const
{
	return getElementsFromBlock("BeforeBlock");
}

IdList RefactoringApplier::getElementsFromAfterBlock() const
{
	return getElementsFromBlock("AfterBlock");
}

IdList RefactoringApplier::getElementsFromBlock(QString const &blockType) const
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

IdList RefactoringApplier::getElementsFromActiveDiagram() const
{
	Id const activeDiagram = mInterpretersInterface.activeDiagram();
	return children(activeDiagram);
}

Id RefactoringApplier::idElementWithID(QString const &IDValue, IdList const &idList)
{
	foreach (Id const &id, idList) {
		if (mRefactoringRepoApi->property(mRefactoringRepoApi->logicalId(id), "ID").toString() == IDValue)
			return id;
	}
	return Id::rootId();
}

void RefactoringApplier::addElement(Id const &id, IdList *idList)
{
	if (!idList->contains(id))
		idList->append(id);
}

void RefactoringApplier::loadRefactoringRule()
{
	IdList const before = getElementsFromBeforeBlock();
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

bool RefactoringApplier::compareLinks(Id const &first,Id const &second) const
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

bool RefactoringApplier::compareElements(Id const &first, Id const &second) const
{
	return compareElementTypesAndProperties(first, second);
}

bool RefactoringApplier::compareElementTypesAndProperties(Id const &first,
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

IdList RefactoringApplier::linksModel(Id const &id) const
{
	return mLogicalModelApi.logicalRepoApi().links(id);
}

IdList RefactoringApplier::children(Id const &id) const
{
	return mGraphicalModelApi.graphicalRepoApi().children(id);
}

Id RefactoringApplier::getLinkEndModel(const qReal::Id &linkInModel, const qReal::Id &nodeInModel) const
{
	Id const linkTo = toInModel(linkInModel);
	if (linkTo == nodeInModel) {
		return fromInModel(linkInModel);
	}
	return linkTo;
}

bool RefactoringApplier::isEdgeInModel(Id const &element) const
{
	return toInModel(element) != Id::rootId() ||
			fromInModel(element) != Id::rootId();
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

Id RefactoringApplier::getStartElement()
{
	IdList const applyElementsTo = getApplyElementsTo();
	foreach (Id const &id, applyElementsTo) {
	if (!isEdgeInModel(id))
		return id;
	}
	return Id::rootId();
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
		changeElement(mApply->at(i).second, mApply->at(i).first);
	}
}

void RefactoringApplier::changeElement(Id const &changeFromId, Id const &changeToId)
{
	Id const beforeId = beforeIdInRule(changeToId);
	if (isElementTypesInRuleIdentical(beforeId, changeToId)) {
		if (changeToId.element() == "Element") {
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
		else {
			changePropertiesInModel(changeFromId, changeToId);
		}
	}
	else {
		changeElementInModel(changeFromId, changeToId);
	}
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
