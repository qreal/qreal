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
	mDefaultProperties.insert("ID");
}

IdList RefactoringFinder::elementsFromBeforeBlock() const
{
	return elementsFromBlock("BeforeBlock");
}

IdList RefactoringFinder::elementsFromAfterBlock() const
{
	return elementsFromBlock("AfterBlock");
}

IdList RefactoringFinder::elementsFromBlock(QString const &blockType) const
{
	IdList list;
	IdList const refactoringElements = mRefactoringRepoApi->children(Id::rootId());
	foreach (Id const &refactoringElement, refactoringElements) {
		if (mRefactoringRepoApi->isGraphicalElement(refactoringElement)) {
			if (refactoringElement.element() == "RefactoringDiagramNode") {
				list = mRefactoringRepoApi->children(refactoringElement);
				foreach (Id const &id, list) {
					if (id.element() == blockType) {
						return mRefactoringRepoApi->children(id);
					}
				}
			}
		}
	}
	return IdList();
}

bool RefactoringFinder::containElementWithID(QString const &idValue, IdList const &idList)
{
	foreach (Id const &id, idList) {
		if (mRefactoringRepoApi->property(id, "ID").toString() == idValue) {
			return true;
		}
	}
	return false;
}

void RefactoringFinder::addElement(Id const &id, IdList *idList)
{
	if (!idList->contains(id)) {
		idList->append(id);
	}
}

void RefactoringFinder::highlightMatch()
{
	if (findMatch()) {
		for (int i = 0; i < mMatches.size(); ++i) {
			QHash <Id, Id> currentMatch = mMatches.at(i);
			foreach (Id const &id, currentMatch.keys()) {
				mInterpretersInterface.highlight(currentMatch.value(id), false);
				pause(500);
			}
			pause(500);
			mInterpretersInterface.dehighlight();
			pause(1000);
		}
	} else {
		mInterpretersInterface.errorReporter()->addInformation("Not Found");
	}
}

bool RefactoringFinder::findMatch()
{
	mMatches.clear();
	return checkRuleMatching();
}

Id RefactoringFinder::startElement() const
{
	IdList const before = elementsFromBeforeBlock();

	foreach (Id const &beforeId, before) {
		if (!isEdgeInRule(beforeId)) {
			return beforeId;
		}
	}
	return Id::rootId();
}

bool RefactoringFinder::compareElements(Id const &first, Id const &second) const
{
	return compareElementTypesAndProperties(first, second);
}

bool RefactoringFinder::compareElementTypesAndProperties(Id const &first,
		Id const &second) const
{
	if (first == Id::rootId()) {
		return false;
	}
	
	bool firstIsNode = !isEdgeInModel(first);
	if (second.element() == "Element" && firstIsNode) {
		QString const elementName = mRefactoringRepoApi->name(second);
		if (elementName == "(Element)" || elementName.contains("EXIST")) {
			return true;
		}
		return (elementName == mGraphicalModelApi.name(first));
	}

	if (second.element() == "Link" && !firstIsNode) {
		QString const elementName = mRefactoringRepoApi->name(second);
		if (elementName == "(Link)" || elementName.contains("EXIST")) {
			return true;
		}
		return (elementName == mGraphicalModelApi.name(first));
	}

	return BaseGraphTransformationUnit::compareElementTypesAndProperties(first, second);
}

QMapIterator<QString, QVariant> RefactoringFinder::propertiesIterator(Id const &id) const
{
	return mRefactoringRepoApi->propertiesIterator(id);
}

QVariant RefactoringFinder::refactoringProperty(Id const &id, QString const &propertyName) const
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
	IdList const before = elementsFromBeforeBlock();
	foreach (Id const &beforeId, before) {
		if (beforeId.element() == "SelectedSegment") {
			return true;
		}
	}
	return false;
}
