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

namespace updatesChecker {

/// Checks for new updates on the remote server in background.
class UpdatesCheckerPlugin : public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)
	Q_PLUGIN_METADATA(IID "updatesChecker.UpdatesCheckerPlugin")

public:
	UpdatesCheckerPlugin();

	QList<qReal::ActionInfo> actions() override;

	void init(const qReal::PluginConfigurator &configurator) override;

	QStringList defaultSettingsFiles() override;

private slots:
	void checkForUpdates(bool reportNoUpdates = true);
	void showUpdatesDialog();
	void reportNoUpdates();

private:
	void initSettingsUi(qReal::gui::PreferencesPage &behaviourPage);

	QAction *mSeparator;
	QAction *mCheckForUpdatesAction;
	qReal::ErrorReporterInterface *mErrorReporter;
	QWidget *mMainWindowWidget;
};

}
