#pragma once

#include <QDialog>

#include "../pluginManager/editorManagerInterface.h"

namespace Ui {
	class EdgePropertiesDialog;
}

namespace qReal {

class EdgePropertiesDialog : public QDialog
{
	Q_OBJECT
public:
	explicit EdgePropertiesDialog(MainWindow *mainWindow, Id const &diagram, EditorManagerInterface const *editorManagerProxy, QWidget *parent = 0);
	~EdgePropertiesDialog();
private slots:
	void ok();
private:
	Ui::EdgePropertiesDialog *mUi;
	MainWindow *mMainWindow;
	Id const mDiagram;
	EditorManagerInterface const *mEditorManagerProxy;
};
}
