#pragma once
#include <QDialog>
#include <QKeyEvent>

#include "startDialog/suggestToCreateDiagramWidget.h"

namespace qReal {

class MainWindow;

/**
 * @brief Dialog for create a diagram
 *
 * This dialog prompts the user to create one of the available diagrams
 */
class SuggestToCreateDiagramDialog : public QDialog
{
	Q_OBJECT

public:
	explicit SuggestToCreateDiagramDialog(MainWindow *mainWindow, bool isNonClosable = false);
	void keyPressEvent(QKeyEvent *event);

private:
	bool mIsNonClosable;
};

}
