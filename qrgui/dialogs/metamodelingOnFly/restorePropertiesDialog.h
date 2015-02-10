#pragma once

#include <QtWidgets/QDialog>

#include <qrgui/plugins/pluginManager/editorManagerInterface.h>

#include "qrgui/dialogs/dialogsDeclSpec.h"

namespace Ui {
class RestorePropertiesDialog;
}

namespace qReal {

/// Window where you can restore the property with the same name, which was renamed or removed.
class QRGUI_DIALOGS_EXPORT RestorePropertiesDialog : public QDialog
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param parent Parent widget, who is responsible for deletion of this dialog.
	/// @param editorManagerProxy Editor manager.
	explicit RestorePropertiesDialog(QWidget *parent, const EditorManagerInterface &editorManagerProxy);

	/// Destructor.
	~RestorePropertiesDialog();

	/// Filling the same name properties table widget.
	/// @param propertiesWithTheSameNameList IdList of elements with the same name.
	/// @param propertyName name of property.
	void fillSameNamePropertiesTW(const IdList &propertiesWithTheSameNameList, QString  const &propertyName);

signals:
	void createNewChosen();

private slots:
	void restoreButtonClicked();
	void createButtonClicked();

private:
	Ui::RestorePropertiesDialog *mUi;
	const EditorManagerInterface &mInterpreterEditorManager;
	IdList mPropertiesWithTheSameNameList;
};
}
