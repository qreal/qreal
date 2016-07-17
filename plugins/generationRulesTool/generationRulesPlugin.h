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
#include <qrgui/plugins/pluginManager/editorManagerInterface.h>

#include "ast/node.h"

#include "dialogs/specifyGenerationRulesDialog.h"
#include "dialogs/specifyPathToGeneratedCodeDialog.h"

namespace generationRules {

/// Class, which represents example of using interpreted plugin.
class GenerationRulesPlugin : public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)
	Q_PLUGIN_METADATA(IID "GenerationRulesPlugin")

public:
	/// Constructor of class GenerationRulesPlugin.
	GenerationRulesPlugin();
	~GenerationRulesPlugin() override;

	QList<qReal::ActionInfo> actions() override;
	/// @todo: This is currently ignored by engine. Extend it!
	QList<QAction *> menuActionList() const;

	void init(const qReal::PluginConfigurator &configurator) override;

private slots:
	void openWindowForPathsSpecifying();

	void generateCodeForAllElements();

	void generateCode(
			const qReal::Id &editorId
			, const qReal::Id &diagramId);

	void openGenerationRulesWindow();

private:
	qrRepo::RepoControlInterface *mRepo;  // Doesn't have ownership
	QAction mAction;  // "Generate" button

	/// To syncronize logical and graphical models with repository.
	qReal::gui::MainWindowInterpretersInterface *mMainWindowInterpretersInterface;  // Doesn't have ownership

	qReal::LogicalModelAssistInterface *mLogicalModelAssistInterface;  // Doesn't have ownership.

	const qReal::EditorManagerInterface *mEditorManagerInterface;  // Doesn't have ownership

	qReal::Id mRootId;

	qReal::gui::SpecifyPathToGeneratedCodeDialog *mSpecifyPathsDialog;  // Doesn't have ownership.

	QString mPathToGeneratedCode;
	QString mMainFileName;
};

}
