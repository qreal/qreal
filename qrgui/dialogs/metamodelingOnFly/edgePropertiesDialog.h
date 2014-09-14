#pragma once

#include <QtWidgets/QDialog>

#include "restoreElementDialog.h"
#include "plugins/pluginManager/editorManagerInterface.h"

namespace Ui {
class EdgePropertiesDialog;
}

namespace qReal {
namespace gui {

/// Dialog for specification of edge metaproperties.
class EdgePropertiesDialog : public QDialog
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param mainWindow Reference to QReal main window.
	/// @param diagram Id of a diagram to which we will add new edge.
	/// @param editorManagerProxy Editor manager.
	explicit EdgePropertiesDialog(MainWindow &mainWindow, Id const &diagram
			, EditorManagerInterface const &editorManagerProxy);

	/// Destructor.
	~EdgePropertiesDialog();

private slots:
	void okButtonClicked();
	void addEdgeElement();

private:
	Ui::EdgePropertiesDialog *mUi;
	MainWindow &mMainWindow;
	Id const mDiagram;
	QString mEdgeName;
	EditorManagerInterface const &mEditorManagerProxy;
	RestoreElementDialog *mRestoreElementDialog;  // Doesn`t take ownership
};

}
}
