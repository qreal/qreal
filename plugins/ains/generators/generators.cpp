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

#include "generators.h"

#include <QtWidgets/QApplication>

using namespace qReal;
using namespace ains;

AinsPlugin::AinsPlugin()
{
}

AinsPlugin::~AinsPlugin()
{
}

void AinsPlugin::init(PluginConfigurator const &configurator)
{
	mDatabaseEditorGenerator = new DatabaseEditorSchemeGenerator(configurator.logicalModelApi()
			, *configurator.mainWindowInterpretersInterface().errorReporter());
}

qReal::Customizer* AinsPlugin::customizationInterface()
{
	return nullptr;
}

QList<ActionInfo> AinsPlugin::actions()
{
	mGenerateDatabaseSchemeAction = new QAction (QObject::tr("generate Database Scheme"), nullptr);
	ActionInfo DatabaseSchemeActionInfo(mGenerateDatabaseSchemeAction, "interpreters", "tools");
	QObject::connect(mGenerateDatabaseSchemeAction, SIGNAL(triggered()), this, SLOT(generateDatabaseScheme()));

	QAction *separator = new QAction(nullptr);
	ActionInfo separatorActionInfo(separator, "interpreters", "tools");
	separator->setSeparator(true);

	updateSettings();

	return QList<ActionInfo>() << DatabaseSchemeActionInfo << separatorActionInfo;
}

QPair<QString, gui::PreferencesPage *> AinsPlugin::preferencesPage()
{
	return {QString(), nullptr};
}

void AinsPlugin::updateSettings()
{

}

void AinsPlugin::generateDatabaseScheme()
{

	QHash<Id, QPair<QString, QString> > modelList = mDatabaseEditorGenerator->modelList();
	foreach (Id id, modelList.keys()) {
		QString path = modelList[id].first + "/" + modelList[id].second;
		mDatabaseEditorGenerator->generateDatabaseScheme(id, path);
	}
}

