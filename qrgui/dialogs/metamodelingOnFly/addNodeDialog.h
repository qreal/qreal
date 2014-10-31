#pragma once

#include <QtWidgets/QDialog>

#include "restoreElementDialog.h"
#include "plugins/pluginManager/editorManagerInterface.h"

namespace Ui {
class AddNodeDialog;
}

namespace qReal {
namespace gui {

/// Dialog for adding new node to interpreted language.
class AddNodeDialog : public QDialog
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param diagram Id of a diagram to which we will add new node.
	/// @param editorManagerProxy Editor manager.
	explicit AddNodeDialog(Id const &diagram, EditorManagerInterface const &editorManagerProxy, QWidget *parent = 0);

	/// Destructor.
	~AddNodeDialog();

signals:
	/// Emitted when node is added and editors must be reloaded.
	void jobDone();

private slots:
	void okButtonClicked();
	void addNode();

private:
	Ui::AddNodeDialog *mUi;
	Id const mDiagram;
	QString mNodeName;
	EditorManagerInterface const &mEditorManagerProxy;
	RestoreElementDialog *mRestoreElementDialog;  // Doesn`t have ownership
};

}
}
