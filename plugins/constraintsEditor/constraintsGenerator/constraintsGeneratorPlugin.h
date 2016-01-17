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
	void init(qReal::PluginConfigurator const &configurator) override;

	/// Returns list containing "generate" action, which runs a generator.
	QList<qReal::ActionInfo> actions() override;

private slots:
	void generate();
private:
	/// Compiles and loads to QReal new editor plugin
	void loadNewEditor(
			QString const &directoryName
			///< Directory where editor was generated (containing .pro file of that constraint)
			, QPair<QString, QString> const &pluginsNames /// ConstraintsPluginNames
			, QString const &pluginId /// ConstraintsPluginId
			, QString const &commandFirst  ///< qmake command
			, QString const &commandSecond  ///< make command
			, QString const &extension  ///< Extension of the resulting binary file with editor plugin (.dll/.so)
			, QString const &prefix
			///< Optional OS-dependent prefix for resulting binary file (lib for linux, for example)
			, QString const &buildConfiguration ///< debug or release, passed to CONFIG variable of qmake
			);

	void deleteGeneratedFiles(QString const &directoryName, QString const &fileBaseName);//qwerty_deleteFiles

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
