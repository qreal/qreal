#ifndef PROPERTYEDITORDELEGATE_H
#define PROPERTYEDITORDELEGATE_H

#include <QItemDelegate>
#include <QModelIndex>
#include <QObject>
#include <QSize>
#include <QLineEdit>

class PropertyEditorDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    PropertyEditorDelegate(QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
           const QStyleOptionViewItem &option, const QModelIndex &index) const;

};

#endif
