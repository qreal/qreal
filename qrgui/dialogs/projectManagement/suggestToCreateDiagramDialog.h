#pragma once

#include "dialogs/projectManagement/managedClosableDialog.h"

namespace qReal {

class EditorManagerInterface;

/// @brief Dialog for create a diagram
/// This dialog prompts the user to create one of the available diagrams
class SuggestToCreateDiagramDialog : public ManagedClosableDialog
{
	Q_OBJECT

public:
	explicit SuggestToCreateDiagramDialog(EditorManagerInterface const &editorManager
			, QWidget *parent = 0, bool isClosable = true);

signals:
	/// Emitted when user selects some item in diagrams list wishing thus to create
	/// new diagram of the type @arg diagramIdString.
	void diagramSelected(QString const &diagramIdString);
};

}
