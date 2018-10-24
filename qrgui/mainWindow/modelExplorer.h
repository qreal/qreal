/* Copyright 2007-2016 QReal Research Group
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

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/editorInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/details/modelsAssistInterface.h>

namespace qReal {

class Controller;
namespace models {
class Clipboard;
class Exploser;
class Models;
}

namespace gui {

/// A graphical tree-like explorer of graphical or logical model (no matter which).
class ModelExplorer : public QTreeView, public EditorInterface
{
	Q_OBJECT

public:
	explicit ModelExplorer(QWidget *parent = nullptr);
	~ModelExplorer();

	/// Configures models explorer to use given external system components.
	void initialize(Controller &controller, const models::Models &models, details::ModelsAssistInterface &model);

	/// Replaces current set of editor actions  with new one.
	/// Editor actions will be shown on context menu and their shortcuts will be triggered
	/// when model explorers are active.
	void changeEditorActionsSet(const QList<QAction *> &actions);

	QString editorId() const override;
	bool supportsCopying() const override;
	bool supportsPasting() const override;
	bool supportsCutting() const override;

public slots:
	void copy() override;
	void paste() override;
	void cut() override;

signals:
	/// Emitted when user wants to remove an element from the model.
	/// @todo: The implementation of the deletion process is currently placed to main window.
	/// Model explorers must be an independent components (maybe even pluginized).
	void elementRemoved();

protected:
	void commitData(QWidget *editor) override;
	void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) override;
	void focusInEvent(QFocusEvent *event) override;
	void focusOutEvent(QFocusEvent *event) override;

private:
	void updateActions();

	Controller *mController;  // Doesn`t have ownership
	details::ModelsAssistInterface *mModel;  // Doesn`t have ownership
	models::Exploser *mExploser;  // Doesn`t have ownership
	QScopedPointer<models::Clipboard> mClipboard;
	QAction mDeleteAction;
};

}
}
