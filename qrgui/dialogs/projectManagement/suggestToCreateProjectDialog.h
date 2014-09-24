#pragma once

#include <QtWidgets/QDialog>

namespace qReal {

class EditorManagerInterface;

/// @brief Dialog for creating a new project.
/// This dialog prompts the user to select one of the available diagrams
/// and new project with selected diagram will be created
class SuggestToCreateProjectDialog : public QDialog
{
	Q_OBJECT

public:
	explicit SuggestToCreateProjectDialog(EditorManagerInterface const &editorManager, QWidget *parent = 0);

signals:
	/// Emitted when user selects some item in diagrams list wishing thus to create
	/// new project with root diagram of the type @arg diagramIdString.
	void diagramSelected(QString const &diagramIdString);
};

}
