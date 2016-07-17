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

#include "compilerPage.h"

namespace metaEditor {

/// Main plugin class for metaeditor support code. Provides generation of editor
/// by metamodel specified in .xml file or in QReal save file, compilation of
/// generated editor and loading it to QReal, and parsing of existing .xml metamodel.
class MetaEditorSupportPlugin : public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)
	Q_PLUGIN_METADATA(IID "metaEditor.MetaEditorSupportPlugin")

public:
	MetaEditorSupportPlugin();
	virtual ~MetaEditorSupportPlugin();

	virtual void init(qReal::PluginConfigurator const &configurator);
	virtual QList<qReal::ActionInfo> actions();
	virtual QPair<QString, qReal::gui::PreferencesPage *> preferencesPage();

private slots:
	/// Generates .xml file with metamodel for later compilation with qrxc tool
	void generateEditorForQrxc();

	/// Generates .cpp/.h code ready to be compiled into editor plugin
	void generateEditorWithQrmc();

	/// Parses existing .xml with metamodel and shows it in QReal as logical model
	/// in metaeditor
	void parseEditorXml();

private:
	/// Compiles and loads to QReal new editor plugin
	void loadNewEditor(
			/// Directory where editor was generated (containing .pro file of that editor)
			QString const &directoryName
			/// Metamodel name as set in metamodel
			, QPair<QString, QString> const &metamodelNames
			/// qmake command
			, QString const &commandFirst
			/// make command
			, QString const &commandSecond
			/// Extension of the resulting binary file with editor plugin (.dll/.so)
			, QString const &extension
			/// Optional OS-dependent prefix for resulting binary file (lib for linux, for example)
			, QString const &prefix
			/// debug or release, passed to CONFIG variable of qmake
			, QString const &buildConfiguration
			);

	/// Action that starts generation of .xml file with metamodel for qrxc
	QAction mGenerateEditorForQrxcAction;

	/// Action that starts generation of C++ sources with qrmc
	QAction mGenerateEditorWithQrmcAction;

	/// Action that starts parsing of existing .xml metamodel
	QAction mParseEditorXmlAction;

	/// Interface of MainWindow, used, for example, to reinit models when finished parsing
	qReal::gui::MainWindowInterpretersInterface *mMainWindowInterface;  // Does not have ownership

	/// API of logical part of a repository, mutable to allow parser build model
	qrRepo::LogicalRepoApi *mLogicalRepoApi;  // Does not have ownership

	/// Control API of a repository, used for requests such as current save file name
	qrRepo::RepoControlInterface *mRepoControlApi;  // Does not have ownership

	/// Page with plugin settings. Created here, but then ownership is passed to
	/// a caller of preferencesPage().
	PreferencesCompilerPage *mCompilerSettingsPage;  // Does not have ownership
};

}
