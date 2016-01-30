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
#include <qrgui/plugins/pluginManager/editorManagerInterface.h>
#include <qrgui/plugins/constraintsPluginInterface/constraintsPluginInterface.h>

namespace constraints {

class ConstraintsAllLanguages
{
public:
	ConstraintsAllLanguages();
	virtual ~ConstraintsAllLanguages();

	qReal::CheckStatus checkAllEdges_one(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi);

	QList<qReal::CheckStatus> checkAllEdges(qReal::Id const &element, qrRepo::LogicalRepoApi const &logicalApi);

	QList<qReal::CheckStatus> check(
			qReal::Id const &element
			, qrRepo::LogicalRepoApi const &logicalApi
			, qReal::EditorManagerInterface const &editorManager);

	QString languageName() const;
	QList<QString> elementsNames() const;

private:
	qReal::EditorManagerInterface const *mEditorManager;
};

}

