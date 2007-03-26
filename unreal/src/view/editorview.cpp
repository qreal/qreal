#include "editorview.h"

EditorView::EditorView(QWidget *parent)
    : QGraphicsView(parent)
{
    setScene(new EditorViewScene(this));

    mvciface = new EditorViewMVCIface(this);
    mvciface->viewport()->hide();
    mvciface->hide();
}

EditorView::~EditorView()
{
}
