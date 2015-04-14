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

#include <qrgui/plugins/pluginManager/editorManagerInterface.h>

#include "qrgui/dialogs/dialogsDeclSpec.h"

namespace Ui {
class ChooseTypeDialog;
}

namespace qReal {
namespace gui {

/// Dialog for metatype selection.
class QRGUI_DIALOGS_EXPORT ChooseTypeDialog : public QDialog
{
	Q_OBJECT

public:

	/// Constructor.
	/// @param diagram Id of a diagram to which we will add new node or edge.
	/// @param editorManagerProxy Editor manager.
	ChooseTypeDialog(const Id &diagram, const EditorManagerInterface &editorManagerProxy, QWidget *parent = 0);

	/// Destructor.
	~ChooseTypeDialog();

signals:
	/// Emitted when editing is complete and everything must be reloaded.
	void jobDone();

private slots:
	void okButtonClicked();

private:
	Ui::ChooseTypeDialog *mUi;
	const Id mDiagram;
	const EditorManagerInterface &mEditorManagerProxy;
};

}
}
