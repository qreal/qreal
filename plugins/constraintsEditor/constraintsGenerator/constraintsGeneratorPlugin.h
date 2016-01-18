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

#include <QtCore/QTranslator>
#include <qrgui/plugins/toolPluginInterface/toolPluginInterface.h>
#include "generator.h"

namespace constraints {
namespace generator {

class ConstraintsGeneratorPlugin : public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)
	Q_PLUGIN_METADATA(IID "constraints.generator.ConstraintsGeneratorPlugin")

public:
	ConstraintsGeneratorPlugin();
	~ConstraintsGeneratorPlugin() override;

	/// Initializes generator with ref to logical model.
	void init(const qReal::PluginConfigurator &configurator) override;

	/// Returns list containing "generate" action, which runs a generator.
	QList<qReal::ActionInfo> actions() override;

private slots:
	void generate();
private:
	/// Compiles and loads to QReal new editor plugin
	/// @param directoryName - directory where editor was generated (containing .pro file).
	/// @param pluginsNames - constraints plugin names.
	/// @param pluginId - constraints plugin id.
	/// @param commandFirst - qmake command.
	/// @param commandSecond - make command.
	/// @param extension - extension of resulting binary file with editor plugin (.dll/.so).
	/// @param prefix - optional OS-dependent prefix for resulting binary file (lib for linux, for example).
	/// @param buildConfiguration - debug or release, passed to CONFIG variable of qmake.
	void loadNewEditor(
			const QString &directoryName
			, const QPair<QString, QString> &pluginsNames
			, const QString &pluginId
			, const QString &commandFirst
			, const QString &commandSecond
			, const QString &extension
			, const QString &prefix
			, const QString &buildConfiguration
			);

	void deleteGeneratedFiles(
			const QString &directoryName
			, const QString &fileBaseName);

	Generator mGenerator;

	/// Interface of MainWindow, used, for example, to reinit models when finished parsing
	qReal::gui::MainWindowInterpretersInterface *mMainWindowInterface;  // Does not have ownership

	/// Logical model, from which generator takes all information.
	qReal::LogicalModelAssistInterface const *mLogicalModel;

	/// Thanslator object for this plugin, provides localisation
	QTranslator mAppTranslator;
};

}
}
