#pragma once

#include <QtWidgets/QTreeWidget>

#include "paletteTree.h"
#include "../pluginManager/editorManagerInterface.h"

namespace qReal{
namespace gui{

/// Represents single element of a palette corresponding to a language element.
class PaletteTreeWidget : public QTreeWidget
{
	Q_OBJECT

public:
	PaletteTreeWidget(PaletteTree &parent, MainWindow &mainWindow, EditorManagerInterface const &editorManagerProxy);

private slots:
	void addElementPaletteActionTriggered();

private:
	MainWindow &mMainWindow;
	EditorManagerInterface const &mEditorManagerProxy;
	PaletteTree &mPaletteTree;

	void mousePressEvent(QMouseEvent *event);
};
}
}
