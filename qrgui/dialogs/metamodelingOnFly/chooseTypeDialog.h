#pragma once

#include <QtWidgets/QDialog>

#include <qrgui/plugins/pluginManager/editorManagerInterface.h>

namespace Ui {
class ChooseTypeDialog;
}

namespace qReal {
namespace gui {

/// Dialog for metatype selection.
class ChooseTypeDialog : public QDialog
{
	Q_OBJECT

public:

	/// Constructor.
	/// @param diagram Id of a diagram to which we will add new node or edge.
	/// @param editorManagerProxy Editor manager.
	ChooseTypeDialog(Id const &diagram, EditorManagerInterface const &editorManagerProxy, QWidget *parent = 0);

	/// Destructor.
	~ChooseTypeDialog();

signals:
	/// Emitted when editing is complete and everything must be reloaded.
	void jobDone();

private slots:
	void okButtonClicked();

private:
	Ui::ChooseTypeDialog *mUi;
	Id const mDiagram;
	EditorManagerInterface const &mEditorManagerProxy;
};

}
}
