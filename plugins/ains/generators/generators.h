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

#include <qrgui/plugins/toolPluginInterface/toolPluginInterface.h>
#include <qrgui/plugins/toolPluginInterface/pluginConfigurator.h>
#include "databaseSchemeGenerator/databaseSchemeGenerator.h"

namespace qReal {
namespace ains {

class AinsPlugin : public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)

public:
	AinsPlugin();
	virtual ~AinsPlugin();

	virtual void init(PluginConfigurator const &configurator);
	virtual QList<ActionInfo> actions();
	virtual QPair<QString, gui::PreferencesPage *> preferencesPage();
	virtual qReal::Customizer* customizationInterface();
	virtual void updateSettings();

public slots:
	void generateDatabaseScheme();

private:
	QAction *mGenerateDatabaseSchemeAction;
	DatabaseEditorSchemeGenerator *mDatabaseEditorGenerator;

};

}
}
