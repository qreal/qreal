#pragma once

#include <QDialog>

#include "../pluginManager/interpreterEditorManager.h"
#include "../mainwindow/mainWindow.h"
#include "editPropertiesDialog.h"

namespace Ui {
	class PropertiesDialog;
}

namespace qReal {

/// Window where you can edit the properties of the selected item
class PropertiesDialog : public QDialog
{
	Q_OBJECT

public:
	PropertiesDialog(MainWindow *mainWindow, QWidget *parent = 0);
	~PropertiesDialog();
	void init(EditorManagerInterface* interperterEditorManager, Id const &id);

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

	Ui::PropertiesDialog *mUi;
	EditorManagerInterface *mInterperterEditorManager;
	Id mId;
	MainWindow *mMainWindow;
	EditPropertiesDialog *mEditPropertiesDialog;
};
}
