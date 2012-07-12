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
	foreach(Id const &editor, mMainWindow->manager()->editors()) {
		Id editorTmpId = Id::loadFromString("qrm:/" + editor.editor());
		foreach(Id const &diagram, mMainWindow->manager()->diagrams(editorTmpId)) {
			addItem(editor, diagram);
		}
	}
	connect(this, SIGNAL(userDataSelected(QString)), parent, SLOT(close()));
}

void SuggestToCreateDiagramWidget::addItem(const Id &editor, const Id &diagram)
{
	EditorInterface *editorInterface = mMainWindow->mEditorManager.editorInterface(editor.editor());

	QString const diagramName = editorInterface->diagramName(diagram.diagram());
	QString const diagramNodeName = editorInterface->diagramNodeName(diagram.diagram());

	if (diagramNodeName.isEmpty()) {
		return;
	}
	ListWidget::addItem(diagramName,
			"qrm:/" + editor.editor() + "/" + diagram.diagram() + "/" + diagramNodeName,
			"editor: " + editor.editor() + ", diagram: " + diagram.diagram());
}
