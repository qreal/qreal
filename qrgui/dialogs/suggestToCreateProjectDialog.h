#pragma once

#include "dialogs/managedClosableDialog.h"
#include "mainWindow/startWidget/suggestToCreateDiagramWidget.h"

namespace qReal {

class MainWindow;

/// @brief Dialog for creating a new project.
/// This dialog prompts the user to select one of the available diagrams
/// and new project with selected diagram will be created
class SuggestToCreateProjectDialog : public QDialog
{
	Q_OBJECT

public:
	explicit SuggestToCreateProjectDialog(MainWindow *mainWindow);
};

}
