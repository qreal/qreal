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

#include "umlCheckerPlugin.h"
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>

using namespace umlChecker;

UmlCheckerPlugin::UmlCheckerPlugin()
	: mRepo(nullptr)
	, mAction(tr("check"), nullptr)
{
	connect(&mAction, &QAction::triggered, this, &UmlCheckerPlugin::check);
}

UmlCheckerPlugin::~UmlCheckerPlugin()
{
}

QList<qReal::ActionInfo> UmlCheckerPlugin::actions()
{
	return { qReal::ActionInfo(&mAction, "file", "tools") };
}

void UmlCheckerPlugin::init(qReal::PluginConfigurator const &configurator)
{
	mRepo = &configurator.repoControlInterface();
	mMainWindowInterpretersInterface = &configurator.mainWindowInterpretersInterface();
}

void UmlCheckerPlugin::check()
{
	if (QMessageBox::question(mMainWindowInterpretersInterface->windowWidget()
			, tr("checking..")
			, QString(tr("Do you want check your work?"))
			, QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
	{
		return;
	}

	QMessageBox::information(nullptr, tr("Information"), tr("Successfully"), tr("Ok"));
}
