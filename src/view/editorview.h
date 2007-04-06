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
    
    void setDEM(DiagramExplorerModel *model) { model2 = model; }  __attribute((deprecated));
    DiagramExplorerModel *getDEM() { return model2; }  __attribute((deprecated));

public slots:
    void toggleAntialiasing(bool);
    void toggleOpenGL(bool);
    void zoomIn();
    void zoomOut();
    
protected:
    void mousePressEvent(QMouseEvent *event);

private:
    EditorViewMViface * mv_iface;
    DiagramExplorerModel *model2 __attribute((deprecated));
};

#endif
