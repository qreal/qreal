#ifndef EXAMPLEEDITOR_H
#define EXAMPLEEDITOR_H

#include <QAbstractItemView>
#include <QFont>
#include <QItemSelection>
#include <QItemSelectionModel>
#include <QModelIndex>
#include <QRect>
#include <QSize>
#include <QPoint>
#include <QWidget>
#include <QPushButton>

#include <QGraphicsScene>
#include <QGraphicsView>

class ExampleEditor : public QAbstractItemView
{
    Q_OBJECT

public:
    ExampleEditor(QWidget *parent = 0);

    QRect visualRect(const QModelIndex &index) const;
    void scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible);
    QModelIndex indexAt(const QPoint &point) const;

    void setModel ( QAbstractItemModel * model );
    
    QGraphicsItem * getItem(int uuid);
    
    void userclickedon ( const QModelIndex & index );

signals:
    void clicked ( const QModelIndex & index );
    
public slots:
    void reset();

protected slots:
    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void rowsAboutToBeRemoved ( const QModelIndex & parent, int start, int end );
    void rowsInserted ( const QModelIndex & parent, int start, int end );
    void selectionChanged ( const QItemSelection & selected, const QItemSelection & deselected );
    
protected:
    bool edit(const QModelIndex &index, EditTrigger trigger, QEvent *event);
    QModelIndex moveCursor(QAbstractItemView::CursorAction cursorAction,
                           Qt::KeyboardModifiers modifiers);

    int horizontalOffset() const;
    int verticalOffset() const;
    
    bool isIndexHidden(const QModelIndex &index) const;

    void setSelection(const QRect&, QItemSelectionModel::SelectionFlags command);

    QRegion visualRegionForSelection(const QItemSelection &selection) const;
    

private:
    void initGraphicsScene();

    void clearScene();
    void dumpStuff(const QModelIndex &idx);

    int rows(const QModelIndex &index = QModelIndex()) const;

    int margin;
    int totalSize;
    int pieSize;
    int validItems;
    double totalValue;
    QPoint origin;
    
    QGraphicsScene *scene;
    QGraphicsView *view;
	    
    QMap<int, QGraphicsItem *> items;    
};

#endif
