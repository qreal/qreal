#include <QtGui/QLayout>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtCore/QDebug>
#include "suggestToCreateDiagramWidget.h"
#include "../../../qrkernel/ids.h"
#include "../mainwindow/mainWindow.h"

using namespace qReal;

SuggestToCreateDiagramWidget::SuggestToCreateDiagramWidget(MainWindow *mainWindow, QDialog *parent)
	: ListWidget(parent)
	, mMainWindow(mainWindow)
{
	foreach(Id editor,
			mMainWindow->manager()->editors()) {
		foreach(Id diagram,
				mMainWindow->manager()->diagrams(Id::loadFromString("qrm:/" + editor.editor()))) {

			EditorInterface *editorInterface = mMainWindow->mEditorManager.editorInterface(editor.editor());
			QString const diagramName = editorInterface->diagramName(diagram.diagram());
			QString const diagramNodeName = editorInterface->diagramNodeName(diagram.diagram());

			if (diagramNodeName.isEmpty()) {
				continue;
			}
			addItem(diagramName,
					"qrm:/" +	editor.editor() + "/" + diagram.diagram() + "/" + diagramNodeName);
		}
	}
}
