#ifndef EDITORVIEWMVCIFACE_H
#define EDITORVIEWMVCIFACE_H

#include <QtGui/QAbstractItemView>

class QGraphicsItem;

class EditorView;
class EditorViewScene;

namespace UML {
    class Element;
};

class EditorViewMViface : public QAbstractItemView
{
    Q_OBJECT

public:
    EditorViewMViface(EditorView *view, EditorViewScene *scene);

    QRect visualRect(const QModelIndex &index) const;
    void scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible);
    QModelIndex indexAt(const QPoint &point) const;

    UML::Element* getItem(int uuid) __attribute__ ((deprecated));
    
public slots:
    void reset();
	void setRootIndex(const QModelIndex &index);

protected slots:
    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void rowsAboutToBeRemoved ( const QModelIndex & parent, int start, int end );
    void rowsInserted ( const QModelIndex & parent, int start, int end );
//    void selectionChanged ( const QItemSelection & selected, const QItemSelection & deselected );
    
//protected:
//    bool edit(const QModelIndex &index, EditTrigger trigger, QEvent *event);

    QModelIndex moveCursor(QAbstractItemView::CursorAction cursorAction,
                           Qt::KeyboardModifiers modifiers);

    int horizontalOffset() const;
    int verticalOffset() const;
    
    bool isIndexHidden(const QModelIndex &index) const;

    void setSelection(const QRect&, QItemSelectionModel::SelectionFlags command);

    QRegion visualRegionForSelection(const QItemSelection &selection) const;
    

private:
//    int rows(const QModelIndex &index = QModelIndex()) const;

//    void clearScene();
//    void dumpStuff(const QModelIndex &idx);

//    int margin;
//    int totalSize;
//    int pieSize;
//    int validItems;
//    double totalValue;
//    QPoint origin;
    
      EditorViewScene *scene;
      EditorView *view;
	    
      QMap<int, UML::Element*> items;    

//    friend class EditorView;
//    friend class EditorViewScene;
};

#endif
