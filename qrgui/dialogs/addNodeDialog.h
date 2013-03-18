#pragma once

#include <QDialog>

#include "../pluginManager/editorManagerInterface.h"

namespace Ui {
	class AddNodeDialog;
}

namespace qReal {

class AddNodeDialog : public QDialog
{
	Q_OBJECT

public:
	explicit AddNodeDialog(MainWindow *mainWindow, Id const &diagram, EditorManagerInterface const *editorManagerProxy, QWidget *parent = 0);
	~AddNodeDialog();
private slots:
	void ok();
private:
	Ui::AddNodeDialog *mUi;
	MainWindow *mMainWindow;
	Id const mDiagram;
	EditorManagerInterface const *mEditorManagerProxy;
};
}
