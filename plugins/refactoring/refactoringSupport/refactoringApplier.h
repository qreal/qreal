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
	RefactoringApplier(LogicalModelAssistInterface &logicalModelApi
			, GraphicalModelAssistInterface &graphicalModelApi
			, gui::MainWindowInterpretersInterface &interpretersInterface
			, qrRepo::RepoApi *refactoringRepoApi
			, QHash<Id, Id> *match);
	~RefactoringApplier();

	void applyRefactoringRule();

private:
	bool hasProperty(const Id &id, const QString &propertyName) const;
	QHash<QString, QVariant> getProperties(const Id &id) const;
	IdList getElementsFromBeforeBlock();
	IdList getElementsFromAfterBlock();
	IdList getElementsFromBlock(const QString &blockType);
	Id idElementWithID(const QString &IDValue, const IdList &idList);
	QVariant getProperty(const Id &id, const QString &propertyName) const;

	void setProperty(const Id &id, const QString &propertyName, const QVariant &value) const;
	Id beforeIdInRule(const Id &id);
	bool isElementTypesInRuleIdentical(const Id &beforeId, const Id &afterId);

	void changePropertiesInModel(const Id &changeFromId, const Id &changeToId);
	void changeElementInModel(const Id &changeFromId, const Id &changeToId);

	QVariant getRefactoringProperty(const Id &id, const QString &propertyName) const;

	void loadRefactoringRule();

	IdList getApplyElementsTo();
	void changeNamesRefactoring();
	void changeElement(const Id &changeFromId, const Id &changeToId);
	void changeElementName(const Id &changeFromId, const Id &changeToId);

	Id fromInModel(const Id &id) const;
	Id toInModel(const Id &id) const;
	Id fromInRule(const Id &id) const;
	Id toInRule(const Id &id) const;

	bool isNodeInModel(const Id &id);
	bool isNodeInRule(const Id &id);

	QString propertyID(const Id &id);
	void checkDirection(const Id &changeFromId, const Id &changeToId, const Id &beforeId);

	gui::MainWindowInterpretersInterface &mInterpretersInterface;
	LogicalModelAssistInterface &mLogicalModelApi;
	GraphicalModelAssistInterface &mGraphicalModelApi;

	QList<QPair<Id, Id> > *mRule;
	QList<QPair<Id, Id> > *mApply;

	qrRepo::RepoApi *mRefactoringRepoApi;
	QHash<Id, Id> *mMatch;
};
}
