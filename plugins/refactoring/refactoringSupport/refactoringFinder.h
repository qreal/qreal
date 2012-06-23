#pragma once

#include "../../../qrkernel/ids.h"

#include "../../../qrgui/mainwindow/errorReporter.h"
#include "../../../qrgui/mainwindow/mainWindowInterpretersInterface.h"
#include "../../../qrutils/graphTransformation/baseGraphTransformationUnit.h"

namespace qReal {

class RefactoringFinder : public BaseGraphTransformationUnit
{
	Q_OBJECT

public:
	RefactoringFinder(LogicalModelAssistInterface const &logicalModelApi
			, GraphicalModelAssistInterface const &graphicalModelApi
			, gui::MainWindowInterpretersInterface &interpretersInterface
			, qrRepo::RepoApi *refactoringRepoApi);

	void highlightMatch();
	bool findMatch();
	bool refactoringRuleContainsSelectedSegment();

private:
	void addElement(const Id &id, IdList *idList);

	IdList getElementsFromBlock(const QString &blockType) const;
	IdList getElementsFromAfterBlock() const;
	IdList getElementsFromBeforeBlock() const;

	Id getStartElement() const;

	QMapIterator<QString, QVariant> getPropertiesIterator(Id const &id) const;
	QVariant getRefactoringProperty(const Id &id, const QString &propertyName) const;
	bool containElementWithID(const QString &IDValue, const IdList &idList);

	bool compareLinks(Id const &first, Id const &second) const;
	bool compareElements(Id const &first, Id const &second) const;
	bool compareElementTypesAndProperties(Id const &first, Id const &second) const;

	Id toInRule(Id const &id) const;
	Id fromInRule(Id const &id) const;
	IdList linksInRule(Id const &id) const;

	qrRepo::RepoApi *mRefactoringRepoApi;
};

}
