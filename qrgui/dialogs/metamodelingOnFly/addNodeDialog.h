#pragma once

#include <QtWidgets/QDialog>

#include "restoreElementDialog.h"
#include "pluginManager/editorManagerInterface.h"

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
	/// @param mainWindow Reference to QReal main window.
	/// @param diagram Id of a diagram to which we will add new node.
	/// @param editorManagerProxy Editor manager.
	explicit AddNodeDialog(MainWindow &mainWindow, Id const &diagram, EditorManagerInterface const &editorManagerProxy);

	/// Destructor.
	~AddNodeDialog();

private slots:
	void okButtonClicked();
	void addNode();

private:
	Ui::AddNodeDialog *mUi;
	MainWindow &mMainWindow;
	Id const mDiagram;
	QString mNodeName;
	EditorManagerInterface const &mEditorManagerProxy;
	RestoreElementDialog *mRestoreElementDialog;
};

}
}
