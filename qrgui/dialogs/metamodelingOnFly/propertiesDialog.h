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
	PropertiesDialog(EditorManagerInterface const &interpreterEditorManager
			, qrRepo::LogicalRepoApi &logicalRepoApi
			, Id const &id
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
	QStringList getPropertiesDisplayedNamesList(QStringList const &propertiesNames);
	void change(QString const &text);
	bool checkElementOnDiagram(qrRepo::LogicalRepoApi const &api, Id &id);
	void findElementsOnDiagram(qrRepo::LogicalRepoApi const &api, Id &id);
	void disableParentProperties(QStringList const propertiesDisplayedNames);

	Ui::PropertiesDialog *mUi;  // Has ownership.
	EditorManagerInterface const &mInterpreterEditorManager;
	qrRepo::LogicalRepoApi &mLogicalRepoApi;
	Id mId;
	QStringList mPropertiesNames;
	qReal::IdList *mElementsOnDiagram;
	EditPropertiesDialog mEditPropertiesDialog;  // Has ownership.
};

}
}
