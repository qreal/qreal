#pragma once

#include <QtWidgets/QDialog>

#include "pluginManager/interpreterEditorManager.h"
#include "mainwindow/mainWindow.h"
#include "dialogs/metamodelingOnFly/editPropertiesDialog.h"

namespace Ui {
class PropertiesDialog;
}

namespace qReal {
namespace gui {

/// Window where you can edit the properties of the selected item
class PropertiesDialog : public QDialog
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param mainWindow Reference to QReal main window.
	/// @param interpreterEditorManager Editor manager.
	/// @param id Id of current metamodel element in which we need to change properties.
	PropertiesDialog(MainWindow &mainWindow, EditorManagerInterface &interpreterEditorManager, Id const &id);

	/// Destructor.
	~PropertiesDialog();

private slots:
	void closeDialog();
	void deleteProperty();
	void addProperty();
	void changeProperty();
	void updatePropertiesNamesList();

private:
	QStringList getPropertiesDisplayedNamesList(QStringList const &propertiesNames);
	void change(QString const &text);
	bool checkElementOnDiagram(qrRepo::LogicalRepoApi const &api, Id &id);
	void findElementsOnDiagram(qrRepo::LogicalRepoApi const &api, Id &id);
	void disableParentProperties(QStringList const propertiesDisplayedNames);

	Ui::PropertiesDialog *mUi;
	EditorManagerInterface &mInterpreterEditorManager;
	Id mId;
	MainWindow &mMainWindow;
	EditPropertiesDialog *mEditPropertiesDialog; // Does not have ownership.
	QStringList mPropertiesNames;
	qReal::IdList *mElementsOnDiagram;
};

}
}
