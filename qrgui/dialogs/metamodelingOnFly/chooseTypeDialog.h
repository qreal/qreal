#pragma once

#include <QtWidgets/QDialog>

#include <qrgui/plugins/pluginManager/editorManagerInterface.h>

#include "qrgui/dialogs/dialogsDeclSpec.h"

namespace Ui {
class ChooseTypeDialog;
}

namespace qReal {
namespace gui {

/// Dialog for metatype selection.
class QRGUI_DIALOGS_EXPORT ChooseTypeDialog : public QDialog
{
	Q_OBJECT

public:

	/// Constructor.
	/// @param diagram Id of a diagram to which we will add new node or edge.
	/// @param editorManagerProxy Editor manager.
	ChooseTypeDialog(const Id &diagram, const EditorManagerInterface &editorManagerProxy, QWidget *parent = 0);

	/// Destructor.
	~ChooseTypeDialog();

signals:
	/// Emitted when editing is complete and everything must be reloaded.
	void jobDone();

private slots:
	void okButtonClicked();

private:
	Ui::ChooseTypeDialog *mUi;
	const Id mDiagram;
	const EditorManagerInterface &mEditorManagerProxy;
};

}
}
