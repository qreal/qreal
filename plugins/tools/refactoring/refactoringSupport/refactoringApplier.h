/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <QtCore/QPair>

#include <qrkernel/ids.h>
#include <qrgui/mainWindow/errorReporter.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/mainWindowInterpretersInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h>
#include <qrrepo/repoApi.h>

namespace qReal {

/// Refactoring applier performs different operations on logical and graphical models
/// according to refactoring
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
	Id subprogramElementId() const;

private:
	IdList elementsFromBeforeBlock() const;
	IdList elementsFromAfterBlock() const;
	IdList elementsFromBlock(const QString &blockType) const;

	Id idElementWithID(const QString &idValue, const IdList &idList);

	bool hasProperty(const Id &id, const QString &propertyName) const;
	QVariant property(const Id &id, const QString &propertyName) const;
	QHash<QString, QVariant> properties(const Id &id) const;
	void setProperty(const Id &id, const QString &propertyName, const QVariant &value) const;

	Id beforeIdInRule(const Id &id);
	bool isElementTypesInRuleIdentical(const Id &beforeId, const Id &afterId);

	void changePropertiesInModel(const Id &changeFromId, const Id &changeToId);
	void changeElementInModel(const Id &changeFromId, const Id &changeToId);

	QVariant refactoringProperty(const Id &id, const QString &propertyName) const;

	void loadRefactoringRule();

	IdList applyElementsTo();
	void changeNamesRefactoring();
	void changeElement(const Id &changeFromId, const Id &changeToId);
	void changeElementName(const Id &changeFromId, const Id &changeToId);

	Id fromInModel(const Id &id) const;
	Id toInModel(const Id &id) const;
	Id fromInRule(const Id &id) const;
	Id toInRule(const Id &id) const;

	bool isNodeInModel(const Id &id) const;
	bool isNodeInRule(const Id &id) const;

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
