#pragma once

#include <QtWidgets/QDialog>

#include <qrgui/plugins/pluginManager/interpreterEditorManager.h>

#include "qrgui/dialogs/metamodelingOnFly/editPropertiesDialog.h"

namespace Ui {
class PropertiesDialog;
}

namespace qReal {
namespace gui {

/// Window where you can edit the properties of the selected item
class QRGUI_DIALOGS_EXPORT PropertiesDialog : public QDialog
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param interpreterEditorManager Editor manager.
	/// @param id Id of current metamodel element in which we need to change properties.
	PropertiesDialog(const EditorManagerInterface &interpreterEditorManager
			, qrRepo::LogicalRepoApi &logicalRepoApi
			, const Id &id
			, QWidget *parent = 0);

	/// Destructor.
	~PropertiesDialog();

private slots:
	void closeDialog();
	void deleteProperty();
	void addProperty();
	void changeProperty();
	void updatePropertiesNamesList();

private:
	QStringList getPropertiesDisplayedNamesList(const QStringList &propertiesNames);
	void change(const QString &text);
	bool checkElementOnDiagram(const qrRepo::LogicalRepoApi &api, Id &id);
	void findElementsOnDiagram(const qrRepo::LogicalRepoApi &api, Id &id);
	void disableParentProperties(const QStringList propertiesDisplayedNames);

	Ui::PropertiesDialog *mUi;  // Has ownership.
	const EditorManagerInterface &mInterpreterEditorManager;
	qrRepo::LogicalRepoApi &mLogicalRepoApi;
	Id mId;
	QStringList mPropertiesNames;
	qReal::IdList *mElementsOnDiagram;
	EditPropertiesDialog mEditPropertiesDialog;  // Has ownership.
};

}
}
