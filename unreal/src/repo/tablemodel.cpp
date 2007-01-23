#include "tablemodel.h"
#include <QDebug>

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const{
//dbg;
  Qt::ItemFlags f = Qt::ItemIsEnabled;
  if (!index.isValid())
    return f;
  TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
  //if (!item->isTable())
    //f |= Qt::ItemIsEditable;
  return f | Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

bool TableModel::setData(const QModelIndex& index, const QVariant &value, int role){
//dbg;
  if (index.isValid() && role == Qt::EditRole) {
    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    QSqlQuery q;
    q.exec("update " + item->parent()->name + " set name='" + value.toString() + "' where name='" + item->name + "'");
    item->setData(value.toString());
   // qDebug() << q.executedQuery();
    //q.exec("select name from " + item->parent()->name);
    //while(q.next()) qDebug() << q.value(0).toString();
    emit dataChanged(index, index);
    return true;
  }
  return false;
}

void TableModel::reset(const QModelIndex& index){
  qDebug() << "changed";
  QSqlQuery q;
  TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
  if (index.parent().isValid())
    q.exec("select * from " + item->parent()->name + " where name='" + item->name + "'");
  else
    q.exec("select a from b"); // quick hack, to be changed a bit later
  setQuery(q);
  qDebug() << q.executedQuery();
}
