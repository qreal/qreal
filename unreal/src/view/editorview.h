#ifndef EDITORVIEW_H
#define EDITORVIEW_H

#include <QtGui/QGraphicsView>

#include "editorviewscene.h"
#include "editorviewmvciface.h"

class EditorView : public QGraphicsView
{
    Q_OBJECT

public:
    EditorView(QWidget *parent = 0);
    ~EditorView();

    EditorViewMViface * mvcIface() { return mvciface; };

protected:
    void mousePressEvent(QMouseEvent *event);

private:
    EditorViewMViface * mvciface;
};

#endif
