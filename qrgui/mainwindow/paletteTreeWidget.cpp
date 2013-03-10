#include <QtGui/QMouseEvent>
#include <QtGui/QMenu>

#include "paletteTreeWidget.h"
#include "../dialogs/chooseTypeDialog.h"

using namespace qReal;
using namespace gui;

PaletteTreeWidget::PaletteTreeWidget(PaletteTree *parent, MainWindow *mainWindow, EditorManagerInterface *editorManagerProxy)
	: QTreeWidget(parent), mMainWindow(mainWindow), mEditorManagerProxy(editorManagerProxy), mPaletteTree(parent)
{}

void PaletteTreeWidget::addElementPaletteActionTriggered()
{
	ChooseTypeDialog *chooseTypeDialog = new ChooseTypeDialog(mMainWindow, mPaletteTree, mEditorManagerProxy);
	chooseTypeDialog->setModal(true);
	chooseTypeDialog->show();
}

void PaletteTreeWidget::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::RightButton) {
		if (mEditorManagerProxy->isInterpretationMode()) {
			QMenu menu;
			QAction * const addElementPaletteAction = menu.addAction(tr("Add Element"));
			connect(addElementPaletteAction, SIGNAL(triggered()), this, SLOT(addElementPaletteActionTriggered()));
			menu.exec(QCursor::pos());
		}
	}
}
