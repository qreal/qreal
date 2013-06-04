#pragma once

#include <QtWidgets/QDialog>
#include <QtWidgets/QListWidgetItem>

#include "../pluginManager/interpreterEditorManager.h"

namespace Ui {
class EditPropertiesDialog;
}

namespace qReal {

/// Window where you can edit the attributes of the selected property.
class EditPropertiesDialog : public QDialog
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param parent Parent widget, who is responsible for deletion of this dialog.
	explicit EditPropertiesDialog(EditorManagerInterface &interperterEditorManager, Id const &id, QWidget *parent);

	/// Destructor.
	~EditPropertiesDialog();

	/// Selects a property for editing.
	void changeProperty(QListWidgetItem *propertyItem, QString const &propertyName);

private slots:
	void ok();
	void mbCancel();
	void updateProperties();

private:
	void setupDefaultValues();

	Ui::EditPropertiesDialog *mUi;
	EditorManagerInterface &mInterperterEditorManager;
	Id mId;
	QString mPropertyName;
	QListWidgetItem *mPropertyItem;
};
}
