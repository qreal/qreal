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
class RestorePropertiesDialog;
}

namespace qReal {

/// Window where you can restore the property with the same name, which was renamed or removed.
class QRGUI_DIALOGS_EXPORT RestorePropertiesDialog : public QDialog
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param parent Parent widget, who is responsible for deletion of this dialog.
	/// @param editorManagerProxy Editor manager.
	explicit RestorePropertiesDialog(QWidget *parent, const EditorManagerInterface &editorManagerProxy);

	/// Destructor.
	~RestorePropertiesDialog();

	/// Filling the same name properties table widget.
	/// @param propertiesWithTheSameNameList IdList of elements with the same name.
	/// @param propertyName name of property.
	void fillSameNamePropertiesTW(const IdList &propertiesWithTheSameNameList, QString  const &propertyName);

signals:
	void createNewChosen();

private slots:
	void restoreButtonClicked();
	void createButtonClicked();

private:
	Ui::RestorePropertiesDialog *mUi;
	const EditorManagerInterface &mInterpreterEditorManager;
	IdList mPropertiesWithTheSameNameList;
};
}
