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

#include "qrgui/models/details/graphicalModel.h"
#include "qrgui/models/details/logicalModel.h"
#include "qrgui/models/graphicalModelAssistApi.h"
#include "qrgui/models/logicalModelAssistApi.h"
#include "qrgui/models/exploser.h"

namespace qReal {
namespace models {

class QRGUI_MODELS_EXPORT Models
{
public:
	Models(const QString &workingCopy, const EditorManagerInterface &editorManager);
	~Models();

	QAbstractItemModel *graphicalModel() const;
	QAbstractItemModel *logicalModel() const;

	GraphicalModelAssistApi &graphicalModelAssistApi() const;
	LogicalModelAssistApi &logicalModelAssistApi() const;

	qrRepo::RepoControlInterface &repoControlApi() const;

	const qrRepo::LogicalRepoApi &logicalRepoApi() const;
	qrRepo::LogicalRepoApi &mutableLogicalRepoApi() const;

	const qrRepo::GraphicalRepoApi &graphicalRepoApi() const;

	Exploser &exploser() const;

	void reinit();

private:
	models::details::GraphicalModel *mGraphicalModel;
	models::details::GraphicalPartModel *mGraphicalPartModel;
	models::details::LogicalModel *mLogicalModel;
	qrRepo::RepoControlInterface *mRepoApi;
	Exploser *mExploser;
};

}
}
