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

#include "umlCheckerFinder.h"

#include <qrkernel/settingsManager.h>

#include <QEventLoop>
#include <QSet>
#include <QtCore/QDebug>

using namespace qReal;

UmlCheckerFinder::UmlCheckerFinder(
		LogicalModelAssistInterface &logicalModelApi
		, GraphicalModelAssistInterface &graphicalModelApi
		, gui::MainWindowInterpretersInterface &interpretersInterface
		, qrRepo::RepoApi *umlCheckerRepoApi)
		: BaseGraphTransformationUnit(logicalModelApi, graphicalModelApi, interpretersInterface)
		, mUmlCheckerRepoApi(umlCheckerRepoApi)
{
	mDefaultProperties.insert("ID");
}

bool UmlCheckerFinder::findMatch()
{
	return true;
}

Id UmlCheckerFinder::startElement() const
{
	return Id::rootId();
}


QMapIterator<QString, QVariant> UmlCheckerFinder::propertiesIterator(Id const &id) const
{
	return mUmlCheckerRepoApi->propertiesIterator(id);
}


bool UmlCheckerFinder::compareElements(Id const &first, Id const &second) const
{
	return true;
}

bool UmlCheckerFinder::compareElementTypesAndProperties(Id const &first, Id const &second) const
{
	return true;
}


Id UmlCheckerFinder::toInRule(Id const &id) const
{
	return id;
}

Id UmlCheckerFinder::fromInRule(Id const &id) const
{
	return id;
}

IdList UmlCheckerFinder::linksInRule(Id const &id) const
{
	IdList q;
	return q;
}
