#pragma once

#include <QDialog>

#include "../mainwindow/paletteTree.h"

namespace Ui {
class ChooseTypeDialog;
}

namespace qReal {
namespace gui{

class ChooseTypeDialog : public QDialog
{
	Q_OBJECT

public:
	ChooseTypeDialog(MainWindow *mainWindow, PaletteTree const *paletteTree, EditorManagerInterface const *editorManagerProxy, QWidget *parent = 0);
	~ChooseTypeDialog();
private slots:
	void okButtonClicked();
private:
	Ui::ChooseTypeDialog *mUi;
	MainWindow *mMainWindow;
	PaletteTree const *mPaletteTree;
	EditorManagerInterface const *mEditorManagerProxy;
};
}
}
