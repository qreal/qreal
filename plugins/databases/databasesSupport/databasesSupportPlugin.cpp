/* Copyright 2014-2015 Anastasia Semenova
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

#include "databasesSupportPlugin.h"

namespace qReal {
namespace databasesSupport {

DatabasesSupportPlugin::DatabasesSupportPlugin()
	: mPreferencesPage(new DatabasesPreferencesPage())
	, mGenerateCodeAction(NULL)
	, mCheckCorectnessAction(NULL)
{
}

DatabasesSupportPlugin::~DatabasesSupportPlugin()
{
	delete mDatabasesGenerator;
	delete mDatabasesReverseEngineer;
}

void DatabasesSupportPlugin::init(PluginConfigurator const &configurator)
{
	mDatabasesGenerator = new DatabasesGenerator(configurator, mPreferencesPage);
	mDatabasesReverseEngineer = new DatabasesReverseEngineer(configurator, mPreferencesPage);
	initActions();
}

QPair<QString, qReal::gui::PreferencesPage *> DatabasesSupportPlugin::preferencesPage()
{
	return qMakePair(tr("Databases"), static_cast<qReal::gui::PreferencesPage *>(mPreferencesPage));
}

qReal::Customizer* DatabasesSupportPlugin::customizationInterface()
{
	return &mCustomizer;
}

QList<qReal::ActionInfo> DatabasesSupportPlugin::actions()
{
	return mActionInfos;
}

void DatabasesSupportPlugin::initActions()
{
	mCheckCorectnessAction = new QAction(tr("Check correctness"), NULL);
	connect(mCheckCorectnessAction, SIGNAL(triggered()), this, SLOT(checkCorrectness()));

	mGenerateCodeAction = new QAction(tr("Generate SQL"), NULL);
	connect(mGenerateCodeAction, SIGNAL(triggered()), this, SLOT(generateCode()));

	mGeneratePhysicalModelAction = new QAction(tr("Generate physical model"), NULL);
	connect(mGeneratePhysicalModelAction, SIGNAL(triggered()), this, SLOT(generatePhysicalModel()));

	mGenerateSchemaAction = new QAction(tr("Generate schema"), NULL);
	connect(mGenerateSchemaAction, SIGNAL(triggered()), this, SLOT(generateSchema()));

	mDatabasesMenu = new QMenu(tr("Databases"));
	mDatabasesMenu->addAction(mGenerateCodeAction);
	mDatabasesMenu->addAction(mCheckCorectnessAction);
	mDatabasesMenu->addAction(mGeneratePhysicalModelAction);
	mDatabasesMenu->addAction(mGenerateSchemaAction);
	ActionInfo databasesMenuInfo(mDatabasesMenu, "tools");

	mActionInfos << databasesMenuInfo;
}

void DatabasesSupportPlugin::checkCorrectness()
{
	mDatabasesGenerator->checkCorrectness();
}

void DatabasesSupportPlugin::generateCode()
{
	mDatabasesGenerator->generateSQLCode();
}

void DatabasesSupportPlugin::generatePhysicalModel()
{
	mDatabasesGenerator->generatePhysicalModel();
}

void DatabasesSupportPlugin::generateSchema()
{
	mDatabasesReverseEngineer->generateSchema();
}


}
}
