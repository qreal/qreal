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

#include <qrkernel/ids.h>
#include <qrutils/graphUtils/baseGraphTransformationUnit.h>
#include <qrgui/mainWindow/errorReporter.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/mainWindowInterpretersInterface.h>
#include <qrrepo/repoApi.h>

namespace qReal {

/// Refactoring finder correctly looks for all refactoring occurrences and
/// can highlight match
class RefactoringFinder : public BaseGraphTransformationUnit
{
	Q_OBJECT

public:
	RefactoringFinder(LogicalModelAssistInterface &logicalModelApi
			, GraphicalModelAssistInterface &graphicalModelApi
			, gui::MainWindowInterpretersInterface &interpretersInterface
			, qrRepo::RepoApi *refactoringRepoApi);

	void highlightMatch();
	bool findMatch();
	bool refactoringRuleContainsSelectedSegment();

private:
	void addElement(const Id &id, IdList *idList) const;

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
