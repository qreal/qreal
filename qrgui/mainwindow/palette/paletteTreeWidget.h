#pragma once

#include <QtWidgets/QTreeWidget>

#include "../pluginManager/editorManagerInterface.h"

namespace qReal {
namespace gui {

class PaletteTree;

/// Represents tree-like view for elements palette (both editor and user)
class PaletteTreeWidget : public QTreeWidget
{
	Q_OBJECT

public:
	PaletteTreeWidget(PaletteTree &parent, MainWindow &mainWindow, EditorManagerInterface const &editorManagerProxy);

private slots:
	void addElementPaletteActionTriggered();

private:
	void mousePressEvent(QMouseEvent *event);

	MainWindow &mMainWindow;
	EditorManagerInterface const &mEditorManagerProxy;
	PaletteTree &mPaletteTree;
};

}
}
