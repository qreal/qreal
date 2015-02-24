#pragma once

#include <QtWidgets/QDialog>

#include "plugins/pluginManager/interpreterEditorManager.h"
#include "mainwindow/mainWindow.h"
#include "dialogs/metamodelingOnFly/editPropertiesDialog.h"

namespace Ui {
class SpecifyGenerationRulesDialog;
}

namespace qReal {
namespace gui {

/// Window where you can specify generation rules for interpreted diagram element.
class QRGUI_DIALOGS_EXPORT SpecifyGenerationRulesDialog : public QDialog
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param mainWindow Reference to QReal main window.
	/// @param interpreterEditorManager Editor manager.
	/// @param id Id of metamodel element we are specifying generation rules for.
	SpecifyGenerationRulesDialog(MainWindow &mainWindow, EditorManagerInterface &interpreterEditorManager, Id const &id);
	~SpecifyGenerationRulesDialog();

private slots:
	/// Inserts property into the code.
	/// @param propertyName Property to insert.
	void insertPropertyIntoCode(QListWidgetItem* property);

private:
	/// Adds properties to the properties list.
	void addPropertiesList();
	/// Returns properties displayed names list.
	/// @param propertiesNames List of properties.
	QStringList propertiesDisplayedNamesList(QStringList const &propertiesNames);

	Ui::SpecifyGenerationRulesDialog *mUi;  // has ownership
	EditorManagerInterface &mInterpreterEditorManager;
	Id mId;
	QStringList mPropertiesNames;
};

}
}
