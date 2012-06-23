#include "refactoringFinder.h"

#include <QEventLoop>
#include <QSet>
#include <QtCore/QDebug>

using namespace qReal;

RefactoringFinder::RefactoringFinder(
		const LogicalModelAssistInterface &logicalModelApi
		, const GraphicalModelAssistInterface &graphicalModelApi
		, gui::MainWindowInterpretersInterface &interpretersInterface
		, qrRepo::RepoApi *refactoringRepoApi)
		: BaseGraphTransformationUnit(logicalModelApi, graphicalModelApi, interpretersInterface)
		, mRefactoringRepoApi(refactoringRepoApi)
{
	defaultProperties.insert("ID");
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

	bool result = BaseGraphTransformationUnit::compareLinks(first, second);

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

	return BaseGraphTransformationUnit::compareElementTypesAndProperties(first, second);
}

QMapIterator<QString, QVariant> RefactoringFinder::getPropertiesIterator(Id const &id) const
{
	return mRefactoringRepoApi->propertiesIterator(id);
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

IdList RefactoringFinder::linksInRule(Id const &id) const
{
	return mRefactoringRepoApi->links(id);
}

bool RefactoringFinder::refactoringRuleContainsSelectedSegment()
{
	IdList const before = getElementsFromBeforeBlock();
	foreach (Id const &beforeId, before) {
		if (beforeId.element() == "SelectedSegment")
			return true;
	}
	return false;
}
