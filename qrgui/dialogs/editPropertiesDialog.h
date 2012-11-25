#pragma once

#include <QDialog>
#include <QListWidgetItem>
#include "../pluginManager/interpreterEditorManager.h"
#include "managedClosableDialog.h"

namespace Ui {
class EditPropertiesDialog;
}

namespace qReal {

class EditPropertiesDialog : public ManagedClosableDialog
{
	Q_OBJECT

public:
	explicit EditPropertiesDialog(QWidget *parent = 0);
	~EditPropertiesDialog();
	void init(QListWidgetItem *selectedItem, EditorManagerInterface* interperterEditorManager, Id const &id, QString const &propertyName);
private slots:
	void ok();
private:
	void setupDefaultValues();
	Ui::EditPropertiesDialog *mUi;
	EditorManagerInterface *mInterperterEditorManager;
	Id mId;
	QString mPropertyName;
	QListWidgetItem *mSelectedItem;
};
}
