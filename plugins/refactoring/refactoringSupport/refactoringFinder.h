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

	IdList elementsFromBlock(const QString &blockType) const;
	IdList elementsFromAfterBlock() const;
	IdList elementsFromBeforeBlock() const;

	Id startElement() const;

	QMapIterator<QString, QVariant> propertiesIterator(Id const &id) const;
	QVariant refactoringProperty(const Id &id, const QString &propertyName) const;
	bool containElementWithID(const QString &idValue, const IdList &idList);

	bool compareElements(Id const &first, Id const &second) const;
	bool compareElementTypesAndProperties(Id const &first, Id const &second) const;

	Id toInRule(Id const &id) const;
	Id fromInRule(Id const &id) const;
	IdList linksInRule(Id const &id) const;

	qrRepo::RepoApi *mRefactoringRepoApi;
};

}
