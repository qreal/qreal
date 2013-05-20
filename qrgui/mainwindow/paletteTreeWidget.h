#pragma once

#include <QtWidgets/QTreeWidget>

#include "paletteTree.h"
#include "../pluginManager/editorManagerInterface.h"

namespace qReal{
namespace gui{

class PaletteTreeWidget : public QTreeWidget
{
	Q_OBJECT
public:
	PaletteTreeWidget(PaletteTree *parent, MainWindow *mainWindow, EditorManagerInterface *editorManagerProxy);
private slots:
	void addElementPaletteActionTriggered();
private:
	MainWindow *mMainWindow;
	EditorManagerInterface *mEditorManagerProxy;
	PaletteTree *mPaletteTree;
	void mousePressEvent(QMouseEvent *event);
};
}
}
