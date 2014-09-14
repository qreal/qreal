#pragma once

#include <QtWidgets/QDialog>

#include "mainWindow/palette/paletteTree.h"

namespace Ui {
class ChooseTypeDialog;
}

namespace qReal {
namespace gui {

/// Dialog for metatype selection.
class ChooseTypeDialog : public QDialog
{
	Q_OBJECT

public:

	/// Constructor.
	/// @param mainWindow Reference to QReal main window.
	/// @param diagram Id of a diagram to which we will add new node or edge.
	/// @param editorManagerProxy Editor manager.
	ChooseTypeDialog(MainWindow &mainWindow, Id const &diagram, EditorManagerInterface const &editorManagerProxy);

	/// Destructor.
	~ChooseTypeDialog();

private slots:
	void okButtonClicked();

private:
	Ui::ChooseTypeDialog *mUi;
	MainWindow &mMainWindow;
	Id const mDiagram;
	EditorManagerInterface const &mEditorManagerProxy;
};

}
}
