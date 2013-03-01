#include <QtGui/QMouseEvent>
#include <QMenu>
#include "paletteTreeWidget.h"

using namespace qReal;
PaletteTreeWidget::PaletteTreeWidget(QWidget *parent, EditorManagerInterface *editorManagerProxy)
	: QTreeWidget(parent), mEditorManagerProxy(editorManagerProxy)
{}

void PaletteTreeWidget::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::RightButton) {
		if (mEditorManagerProxy->isInterpretationMode()) {
			QMenu menu;
			QAction * const addElementPaletteAction = menu.addAction(tr("Add Element"));
			//connect(addElementPaletteAction, SIGNAL(triggered()), SLOT(addElementPaletteActionTriggered()));
			menu.exec(QCursor::pos());
		}
	}
}
