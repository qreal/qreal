#pragma once

#include <QtWidgets/QDialog>
#include <QtWidgets/QListWidgetItem>

#include "../pluginManager/interpreterEditorManager.h"

namespace Ui {
	class EditPropertiesDialog;
}

namespace qReal {

/// Window where you can edit the attributes of the selected property
class EditPropertiesDialog : public QDialog
{
	Q_OBJECT

public:
	explicit EditPropertiesDialog(QWidget *parent = 0);
	~EditPropertiesDialog();
	void init(QListWidgetItem *selectedItem, EditorManagerInterface* interperterEditorManager, Id const &id, QString const &propertyName);

private slots:
	void ok();
	void mbCancel();
	void updateProperties();

private:
	void setupDefaultValues();

	Ui::EditPropertiesDialog *mUi;
	EditorManagerInterface *mInterperterEditorManager;
	Id mId;
	QString mPropertyName;
	QListWidgetItem *mSelectedItem;
};
}
