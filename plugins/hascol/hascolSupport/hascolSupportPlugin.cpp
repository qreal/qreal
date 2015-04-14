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

#include "hascolSupportPlugin.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QFileDialog>

#include "hascolGenerator.h"
#include "hascolParser.h"

using namespace qReal;
using namespace hascol;

HascolSupportPlugin::HascolSupportPlugin()
		: mGenerateCodeAction(NULL)
		, mParseSourcesAction(NULL)
{
}

HascolSupportPlugin::~HascolSupportPlugin()
{
}

void HascolSupportPlugin::init(PluginConfigurator const &configurator)
{
	mMainWindowInterface = &configurator.mainWindowInterpretersInterface();
	mLogicalRepoApi = &configurator.logicalModelApi().mutableLogicalRepoApi();
}

QList<ActionInfo> HascolSupportPlugin::actions()
{
	mGenerateCodeAction.setText(tr("Generate hascol code"));
	ActionInfo generateCodeActionInfo(&mGenerateCodeAction, "generators", "tools");
	connect(&mGenerateCodeAction, SIGNAL(triggered()), this, SLOT(generateHascolSourceCode()));

	mParseSourcesAction.setText(tr("Parse hascol sources"));
	ActionInfo parseSourcesActionInfo(&mParseSourcesAction, "generators", "tools");
	connect(&mParseSourcesAction, SIGNAL(triggered()), this, SLOT(parseHascolSources()));

	return QList<ActionInfo>() << generateCodeActionInfo << parseSourcesActionInfo;
}

void HascolSupportPlugin::generateHascolSourceCode()
{
	HascolGenerator hascolGenerator(*mLogicalRepoApi, *mMainWindowInterface->errorReporter());
	hascolGenerator.generate();
}

void HascolSupportPlugin::parseHascolSources()
{
	QStringList const fileNames = QFileDialog::getOpenFileNames(NULL, tr("Select Hascol files to parse"), ".", "*.md;;*.*");
	if (fileNames.empty())
		return;

	HascolParser parser(*mLogicalRepoApi, *mMainWindowInterface->errorReporter());
	parser.parse(fileNames);

	mMainWindowInterface->reinitModels();
}
