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
#include <qrrepo/logicalRepoApi.h>

namespace domainAnalysis {

/// Main plugin class for domainAnalysis generator code. Provides language
/// metamodel generator by domain model
class Generator: public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)
	Q_PLUGIN_METADATA(IID "domainAnalysis.Generator")

public:
	Generator();
	virtual ~Generator();
	virtual void init(qReal::PluginConfigurator const &configurator);
	virtual QList<qReal::ActionInfo> actions();

private slots:
	void onGenerateMetamodel();

private:

	/// Action that starts generation the metamodel by domain model
	QAction mGenerateMetamodelAction;

	/// Interface of MainWindow
	qReal::gui::MainWindowInterpretersInterface *mMainWindowInterface;  // Does not have ownership

	/// API of logical part of a repository
	qrRepo::LogicalRepoApi *mLogicalRepoApi;  // Does not have ownership
};
}
