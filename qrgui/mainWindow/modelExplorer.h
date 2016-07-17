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

#include <QtWidgets/QTreeView>
#include <QtWidgets/QAction>

#include "controller/controller.h"
#include "models/exploser.h"
#include "plugins/toolPluginInterface/usedInterfaces/details/modelsAssistInterface.h"

namespace qReal {
namespace gui {

class ModelExplorer : public QTreeView
{
	Q_OBJECT

public:
	explicit ModelExplorer(QWidget *parent = 0);

	void setController(Controller * const controller);
	void setAssistApi(details::ModelsAssistInterface * const model);
	void setExploser(const models::Exploser &exploser);

	/// Replaces current set of editor actions  with new one.
	/// Editor actions will be shown on context menu and their shortcuts will be triggered
	/// when model explorers are active.
	void changeEditorActionsSet(QList<QAction *> const &actions);

signals:
	/// Emitted when user wants to remove an element from the model.
	/// @todo: The implementation of the deletion process is currently placed to main window.
	/// Model explorers must be an independent components (maybe even pluginized).
	void elementRemoved();

protected:
	void commitData(QWidget *editor) override;
	void focusOutEvent(QFocusEvent *event) override;
	void focusInEvent(QFocusEvent *event) override;

private:
	void setActionsEnabled(bool enabled);

	Controller *mController; // Doesn`t take ownership
	details::ModelsAssistInterface *mModel; // Doesn`t take ownership
	const models::Exploser *mExploser; // Doesn`t take ownership
	QAction mDeleteAction;
	QAction mDeleteActionSeparator;
};

}
}
