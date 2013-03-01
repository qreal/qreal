#include <QtGui/QMouseEvent>
#include <QMenu>
#include "paletteTreeWidget.h"

using namespace qReal;
PaletteTreeWidget::PaletteTreeWidget(QWidget *parent, EditorManagerInterface *editorManagerProxy)
	: QTreeWidget(parent), mEditorManagerProxy(editorManagerProxy)
{
	QPalette palette = this->palette();
	palette.setBrush(QPalette::Window, QBrush(Qt::red));
	palette.setBrush(QPalette::Base, QBrush(Qt::red));
	this->setPalette(palette);
}

void PaletteTreeWidget::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::RightButton) {
		if (mEditorManagerProxy->isInterpretationMode()) {
			QMenu menu;
			QAction * const addElementPaletteAction = menu.addAction(tr("Add Element"));
			//connect(deleteElementPaletteAction, SIGNAL(triggered()), SLOT(deleteElementPaletteActionTriggered()));
			//deleteElementPaletteAction->setData(elementId.toVariant());
			menu.exec(QCursor::pos());
		}
	}
}
