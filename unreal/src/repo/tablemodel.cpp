#include "tablemodel.h"
#include <dbg.h>
#include <QDebug>
  
  
TableModel::TableModel(QSqlDatabase& _db){
dbg;
    db = _db;
   
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const{
//dbg;
  Qt::ItemFlags f = Qt::ItemIsEnabled;
  if (!index.isValid())
    return f;
  TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
  return f | Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

bool TableModel::setData(const QModelIndex& index, const QVariant &value, int role){
//dbg;
  if (index.isValid() && role == Qt::EditRole) {
    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    //QSqlQuery q;
    db.exec("update " + item->parent()->getName() + " set name='" + value.toString() + "' where name='" + item->getName() + "'");
    item->setData(value.toString());
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
    q = db.exec("select * from " + item->parent()->getName() + " where name='" + item->getName() + "'");
  else
    q = db.exec("select a from b"); // quick hack, to be changed a bit later
  setQuery(q);
  qDebug() << q.executedQuery();
}

void TableModel::setFocus( QModelIndex& ind ){
dbg;
    
    
}
