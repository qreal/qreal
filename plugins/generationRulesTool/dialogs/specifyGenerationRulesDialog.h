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
#include "mainWindow/mainWindow.h"
#include "dialogs/metamodelingOnFly/editPropertiesDialog.h"

class QTreeWidgetItem;

namespace Ui {
class SpecifyGenerationRulesDialog;
}

namespace qReal {
namespace gui {

/// Window where you can specify generation rules for interpreted diagram element.
class SpecifyGenerationRulesDialog : public QDialog
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param mainWindow Reference to QReal main window.
	/// @param interpreterEditorManager Editor manager.
	/// @param id Id of metamodel element we are specifying generation rules for.
	SpecifyGenerationRulesDialog(EditorManagerInterface *interpreterEditorManager
			, const Id &id
			, qrRepo::LogicalRepoApi *metamodelRepoApi
			, QWidget *parent = 0);

	~SpecifyGenerationRulesDialog() override;

private slots:
	/// Inserts property into the code.
	/// @param property - Property to insert.
	void insertPropertyIntoCode(QListWidgetItem* property);

	/// Inserts template into the code.
	/// @param templateItem - Template clicked.
	void insertTemplateIntoCode(QTreeWidgetItem* templateItem);

	/// Saves generation rule into metamodel.
	void saveGenerationRule();

private:
	/// Adds properties to the properties list.
	void addPropertiesList();

	/// Adds list of templates.
	void addTemplatesList();

	/// Add templates of one type.
	/// @param type - template type.
	/// @param listOfTemplates - list of templates of this type.
	void addOneTypeTemplates(const QString &type, const QStringList &listOfTemplates);

	/// Adds previously added rule.
	void addOldRule();

	/// Returns properties displayed names list.
	/// @param propertiesNames List of properties.
	QStringList propertiesDisplayedNamesList(const QStringList &propertiesNames);

	Ui::SpecifyGenerationRulesDialog *mUi;  // has ownership
	EditorManagerInterface *mInterpreterEditorManager;  // doesn't have ownership
	const Id mId;
	QStringList mPropertiesNames;
	qrRepo::LogicalRepoApi *mMetamodelRepoApi;  // doesn't have ownership
};

}
}
