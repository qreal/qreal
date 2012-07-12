#pragma once
#include <QtGui/QListWidget>

#include "../qrkernel/ids.h"
#include "listWidget.h"

namespace qReal {
class MainWindow;
}

namespace qReal {
/**
 * @brief ListWidget filled with available diagram types
 *
 * Associated with item data are diagram Id
 * When you select an item class manually close the parent dialog
 */
class SuggestToCreateDiagramWidget : public ListWidget
{
	Q_OBJECT

public:
	explicit SuggestToCreateDiagramWidget(MainWindow *mainWindow, QDialog *parent = 0);

private:
	void addItem(Id const &editor, Id const &diagram);

	MainWindow *mMainWindow;
};

}
