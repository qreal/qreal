#pragma once

#include "../../../qrkernel/ids.h"

#include "../../../qrgui/mainwindow/errorReporter.h"
#include "../../../qrgui/mainwindow/mainWindowInterpretersInterface.h"

namespace qReal {

class RefactoringFinder : public QObject
{
	Q_OBJECT

public:
	RefactoringFinder(LogicalModelAssistInterface const &logicalModelApi
			, GraphicalModelAssistInterface const &graphicalModelApi
			, gui::MainWindowInterpretersInterface &interpretersInterface
			, qrRepo::RepoApi *refactoringRepoApi);
	~RefactoringFinder();

	void highlightMatch();
	bool findMatch();
	bool refactoringRuleContainsSelectedSegment();
	QList<QHash<Id, Id> > getMatches();

private:

	bool checkRuleMatching();

	bool checkRuleMatchingRecursively();

	bool checkNodeForAddingToMatch(Id const &nodeInModel, Id const &nodeInRule);
	void addElement(const Id &id, IdList *idList);

	bool checkExistingLinks(Id const &nodeInModel, Id const &nodeInRule,
			QHash<Id, Id> *linksToAddInMatch);

	void rollback();

	Id getOutsideLink(Id const &nodeInRule) const;

	Id getLinkEnd(Id const &linkInRule, Id const &nodeInRule) const;
	Id getLinkEndModel(Id const &linkInModel, Id const &nodeInModel) const;
	Id getProperLink(Id const &nodeInModel, Id const &linkInRule,
			Id const &linkEndInRule) const;

	IdList getProperLinks(Id const &nodeInModel, Id const &linkInRule) const;

	IdList getLinksToMatchedSubgraph(Id const &nodeInRule) const;

	IdList getElementsFromBlock(const QString &blockType) const;
	IdList getElementsFromAfterBlock() const;
	IdList getElementsFromBeforeBlock() const;

	IdList getElementsFromActiveDiagram() const;

	Id getStartElement() const;

	QVariant getProperty(Id const &id, QString const &propertyName) const;
	QVariant getRefactoringProperty(const Id &id, const QString &propertyName) const;
	bool hasProperty(Id const &id, QString const &propertyName) const;
	QHash<QString, QVariant> getProperties(Id const &id) const;
	bool containElementWithID(const QString &IDValue, const IdList &idList);

	bool compareLinks(Id const &first, Id const &second) const;
	bool compareElements(Id const &first, Id const &second) const;
	bool compareElementTypesAndProperties(Id const &first, Id const &second) const;

	bool isEdgeInRule(Id const &element) const;
	bool isEdgeInModel(const Id &element) const;

	Id toInRule(Id const &id) const;
	Id fromInRule(Id const &id) const;
	Id toInModel(Id const &id) const;
	Id fromInModel(Id const &id) const;
	IdList linksModel(Id const &id) const;
	IdList children(Id const &id) const;

	void pause(int const &time);

	gui::MainWindowInterpretersInterface &mInterpretersInterface;
	LogicalModelAssistInterface const &mLogicalModelApi;
	GraphicalModelAssistInterface const &mGraphicalModelApi;

	QHash<Id, Id> *mMatch;
	QList<QHash<Id, Id> > mMatches;

	IdList mCurrentMatchedGraphInRule;
	IdList mNodesHavingOutsideLinks;
	int mPos;
	qrRepo::RepoApi *mRefactoringRepoApi;
};

}
