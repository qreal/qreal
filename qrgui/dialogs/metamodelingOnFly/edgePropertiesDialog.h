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
	/// @param diagram Id of a diagram to which we will add new edge.
	/// @param editorManagerProxy Editor manager.
	explicit EdgePropertiesDialog(const Id &diagram
			, const EditorManagerInterface &editorManagerProxy
			, QWidget *parent = 0);

	/// Destructor.
	~EdgePropertiesDialog();

signals:
	/// Emitted when edge`s properties were modified and everything must be reloaded.
	void jobDone();

private slots:
	void okButtonClicked();
	void addEdgeElement();

private:
	Ui::EdgePropertiesDialog *mUi;
	const Id mDiagram;
	QString mEdgeName;
	const EditorManagerInterface &mEditorManagerProxy;
	RestoreElementDialog *mRestoreElementDialog;  // Doesn`t take ownership
};

}
}
