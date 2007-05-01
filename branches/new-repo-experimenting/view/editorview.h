#ifndef EDITORVIEW_H
#define EDITORVIEW_H

#include <QtGui/QGraphicsView>

#include "editorviewscene.h"
#include "editorviewmviface.h"

class DiagramExplorerModel;

class EditorView : public QGraphicsView
{
    Q_OBJECT

public:
    EditorView(QWidget *parent = 0);
    ~EditorView();

    EditorViewMViface * mvIface() { return mv_iface; };
    
public slots:
    void toggleAntialiasing(bool);
    void toggleOpenGL(bool);
    void zoomIn();
    void zoomOut();
    
protected:
    void mousePressEvent(QMouseEvent *event);

private:
    EditorViewMViface * mv_iface;
};

#endif
