#pragma once

#include "../../../qrkernel/ids.h"

#include "../../../qrgui/mainwindow/errorReporter.h"
#include "../../../qrgui/mainwindow/mainWindowInterpretersInterface.h"

#include <QtCore/QPair>

namespace qReal {

class RefactoringApplier : public QObject
{
	Q_OBJECT

public:
	RefactoringApplier(LogicalModelAssistInterface const &logicalModelApi
			, GraphicalModelAssistInterface &graphicalModelApi
			, gui::MainWindowInterpretersInterface &interpretersInterface
			, qrRepo::RepoApi *refactoringRepoApi
			, QHash<Id, Id> *match);
	~RefactoringApplier();

	void applyRefactoringRule();

private:
	bool hasProperty(const Id &id, const QString &propertyName) const;
	QHash<QString, QVariant> getProperties(const Id &id) const;
	IdList getElementsFromBeforeBlock() const;
	IdList getElementsFromAfterBlock() const;
	IdList getElementsFromBlock(const QString &blockType) const;
	IdList getElementsFromActiveDiagram() const;
	Id idElementWithID(const QString &IDValue, const IdList &idList);
	void addElement(const Id &id, IdList *idList);
	bool isEdgeInModel(const Id &element) const;
	QVariant getProperty(const Id &id, const QString &propertyName) const;

	void setProperty(const Id &id, const QString &propertyName, const QVariant &value) const;
	Id beforeIdInRule(const Id &id);
	bool isElementTypesInRuleIdentical(const Id &beforeId, const Id &afterId);

	void changePropertiesInModel(const Id &changeFromId, const Id &changeToId);
	void changeElementInModel(const Id &changeFromId, const Id &changeToId);

	Id getLinkEndModel(const qReal::Id &linkInModel, const qReal::Id &nodeInModel) const;
	IdList children(const Id &id) const;
	IdList linksModel(const Id &id) const;
	Id fromInModel(const Id &id) const;
	Id toInModel(const Id &id) const;
	Id fromInRule(const Id &id) const;
	Id toInRule(const Id &id) const;
	QVariant getRefactoringProperty(const Id &id, const QString &propertyName) const;

	void loadRefactoringRule();
	bool compareLinks(Id const &first, Id const &second) const;
	bool compareElements(Id const &first, Id const &second) const;
	bool compareElementTypesAndProperties(Id const &first, Id const &second) const;

	IdList getApplyElementsTo();
	Id getStartElement();
	void changeNamesRefactoring();
	void changeElement(const Id &changeFromId, const Id &changeToId);

	gui::MainWindowInterpretersInterface &mInterpretersInterface;
	LogicalModelAssistInterface const &mLogicalModelApi;
	GraphicalModelAssistInterface &mGraphicalModelApi;

	QList<QPair<Id, Id> > *mRule;
	QList<QPair<Id, Id> > *mApply;

	qrRepo::RepoApi *mRefactoringRepoApi;
	QHash<Id, Id> *mMatch;
};

}
