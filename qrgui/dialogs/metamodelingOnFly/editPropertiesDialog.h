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
#include <QtWidgets/QListWidgetItem>

#include <qrgui/plugins/pluginManager/interpreterEditorManager.h>

#include "qrgui/dialogs/metamodelingOnFly/restorePropertiesDialog.h"

namespace Ui {
class EditPropertiesDialog;
}

namespace qReal {

/// Window where you can edit the attributes of the selected property.
class QRGUI_DIALOGS_EXPORT EditPropertiesDialog : public QDialog
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param interpreterEditorManager Editor manager.
	/// @param id Id of an element for which we edit properties.
	/// @param parent Parent widget, who is responsible for deletion of this dialog.
	EditPropertiesDialog(const EditorManagerInterface &interpreterEditorManager
			, qrRepo::LogicalRepoApi &api
			, const Id &id
			, QWidget *parent = 0);

	/// Destructor.
	~EditPropertiesDialog();

	/// Selects a property for editing.
	/// @param propertyItem Item in a list of properties which we edit.
	/// @param propertyName Name of a property which we edit.
	/// @param propertyDisplayedName Displayed name of this property.
	/// @param elementsOnDiagram logical elements on the diagram.
	void changeProperty(QListWidgetItem *propertyItem, const QString &propertyName
				, const QString &propertyDisplayedName, qReal::IdList *elementsOnDiagram);
private slots:
	void okButtonClicked();
	void messageBoxCancel();
	void updateProperties();
	void acceptPropertyModifications();

private:
	enum Mode
	{
		addNew
		, editExisting
	};

	void initDefaultValues();

	Ui::EditPropertiesDialog *mUi;
	const EditorManagerInterface &mInterpreterEditorManager;
	RestorePropertiesDialog *mRestorePropertiesDialog;
	Id mId;
	QString mPropertyName;
	QListWidgetItem *mPropertyItem;
	Mode mMode;
	qrRepo::LogicalRepoApi &mApi;
	qReal::IdList mElementsOnDiagram;
};
}
