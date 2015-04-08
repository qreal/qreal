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

#include <qrgui/plugins/pluginManager/interpreterEditorManager.h>

#include "qrgui/dialogs/metamodelingOnFly/editPropertiesDialog.h"

namespace Ui {
class PropertiesDialog;
}

namespace qReal {
namespace gui {

/// Window where you can edit the properties of the selected item
class QRGUI_DIALOGS_EXPORT PropertiesDialog : public QDialog
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param interpreterEditorManager Editor manager.
	/// @param id Id of current metamodel element in which we need to change properties.
	PropertiesDialog(const EditorManagerInterface &interpreterEditorManager
			, qrRepo::LogicalRepoApi &logicalRepoApi
			, const Id &id
			, QWidget *parent = 0);

	/// Destructor.
	~PropertiesDialog();

private slots:
	void closeDialog();
	void deleteProperty();
	void addProperty();
	void changeProperty();
	void updatePropertiesNamesList();

private:
	QStringList getPropertiesDisplayedNamesList(const QStringList &propertiesNames);
	void change(const QString &text);
	bool checkElementOnDiagram(const qrRepo::LogicalRepoApi &api, Id &id);
	void findElementsOnDiagram(const qrRepo::LogicalRepoApi &api, Id &id);
	void disableParentProperties(const QStringList propertiesDisplayedNames);

	Ui::PropertiesDialog *mUi;  // Has ownership.
	const EditorManagerInterface &mInterpreterEditorManager;
	qrRepo::LogicalRepoApi &mLogicalRepoApi;
	Id mId;
	QStringList mPropertiesNames;
	qReal::IdList *mElementsOnDiagram;
	EditPropertiesDialog mEditPropertiesDialog;  // Has ownership.
};

}
}
