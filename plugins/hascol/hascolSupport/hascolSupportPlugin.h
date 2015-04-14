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

namespace hascol {

/// Main plugin class for Hascol tools. Provides generator and parser for Hascol
/// sources. Parser requires installed CoolKit (http://oops.math.spbu.ru/projects/coolkit)
/// and that COOL_ROOT environment variable is set to root of CoolKit installation.
class HascolSupportPlugin : public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)

public:
	HascolSupportPlugin();
	virtual ~HascolSupportPlugin();

	virtual void init(qReal::PluginConfigurator const &configurator);
	virtual QList<qReal::ActionInfo> actions();

private slots:
	/// Slot that calls generator
	void generateHascolSourceCode();

	/// Slot that asks user about files that need to be parsed and calls hascol parser
	void parseHascolSources();

private:
	/// Action that launches code generator
	QAction mGenerateCodeAction;

	/// Action that launches hascol parser
	QAction mParseSourcesAction;

	/// Interface of MainWindow, used, for example, to reinit models when finished parsing
	qReal::gui::MainWindowInterpretersInterface *mMainWindowInterface;  // Does not have ownership

	/// API of logical part of a repository, mutable to allow parser build model
	qrRepo::LogicalRepoApi *mLogicalRepoApi;  // Does not have ownership
};

}
