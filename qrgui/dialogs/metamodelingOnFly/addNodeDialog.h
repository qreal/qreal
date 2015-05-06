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

#include <QtWidgets/QDialog>

#include "restoreElementDialog.h"
#include "plugins/pluginManager/editorManagerInterface.h"

namespace Ui {
class AddNodeDialog;
}

namespace qReal {
namespace gui {

/// Dialog for adding new node to interpreted language.
class AddNodeDialog : public QDialog
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param diagram Id of a diagram to which we will add new node.
	/// @param editorManagerProxy Editor manager.
	explicit AddNodeDialog(const Id &diagram, const EditorManagerInterface &editorManagerProxy, QWidget *parent = 0);

	/// Destructor.
	~AddNodeDialog();

signals:
	/// Emitted when node is added and editors must be reloaded.
	void jobDone();

private slots:
	void okButtonClicked();
	void addNode();

private:
	Ui::AddNodeDialog *mUi;
	const Id mDiagram;
	QString mNodeName;
	const EditorManagerInterface &mEditorManagerProxy;
	RestoreElementDialog *mRestoreElementDialog;  // Doesn`t have ownership
};

}
}
