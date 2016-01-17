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

#include "constraintsAllLanguages.h"
#include <QtCore/QVariant>

using namespace constraints;

ConstraintsAllLanguages::ConstraintsAllLanguages()
{
}

ConstraintsAllLanguages::~ConstraintsAllLanguages()
{
}

qReal::CheckStatus ConstraintsAllLanguages::checkAllEdges_one(
		qReal::Id const &element
		, qrRepo::LogicalRepoApi const &logicalApi)
{
	bool res = true;

	qReal::Id newBeginNodeName_1 = logicalApi.from(element);
	bool beginNodeRes_2 = true;
	bool beginNodeRes_1 = (newBeginNodeName_1 != qReal::Id::rootId());

	qReal::Id newEndNodeName_1 = logicalApi.to(element);
	bool endNodeRes_2 = true;
	bool endNodeRes_1 = (newEndNodeName_1 != qReal::Id::rootId());

	bool allEdgesRes_1 = ((beginNodeRes_2 && beginNodeRes_1) && (endNodeRes_2 && endNodeRes_1));
	res = allEdgesRes_1;

	return qReal::CheckStatus(res, "", qReal::CheckStatus::warning);
}


QList<qReal::CheckStatus> ConstraintsAllLanguages::checkAllEdges(
		qReal::Id const &element
		, qrRepo::LogicalRepoApi const &logicalApi)
{
	QList<qReal::CheckStatus> checkings;
	checkings.append(checkAllEdges_one(element, logicalApi));

	return qReal::CheckStatus::resultCheckStatusList(checkings);
}

QList<qReal::CheckStatus> ConstraintsAllLanguages::check(
		qReal::Id const &element
		, qrRepo::LogicalRepoApi const &logicalApi
		, qReal::EditorManagerInterface const &editorManager)
{
	QList<qReal::CheckStatus> checkings;
	mEditorManager = &editorManager;
	qReal::EditorManagerInterface::MetaType isNodeOrEdge = mEditorManager->metaTypeOfElement(element);

	if(isNodeOrEdge == qReal::EditorManagerInterface::edge) {
		checkings.append(checkAllEdges(element, logicalApi));
	}

	return qReal::CheckStatus::resultCheckStatusList(checkings);
}

QString ConstraintsAllLanguages::languageName() const
{
	return "AllLanguages";
}

QList<QString> ConstraintsAllLanguages::elementsNames() const
{
	QList<QString> elementsList;
	elementsList.append("AllEdges");

	return elementsList;
}
