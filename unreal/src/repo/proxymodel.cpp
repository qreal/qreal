#ifndef PROXY_H
#define PROXY_H

#include <QDebug>
#include "proxymodel.h"

int Proxy::columnCount( const QModelIndex& ) const{
  return sourceModel()->rowCount();
}

int Proxy::rowCount( const QModelIndex& ) const{
  return sourceModel()->columnCount();
}

QVariant Proxy::data(const QModelIndex& index, int role) const{
  qDebug() << "data:  row " << index.row() << ", col " << index.column() << ", data " << sourceModel()->data(createIndex(index.column(), index.row(), index.internalPointer()), role).toString(); 
  return sourceModel()->data(createIndex(index.column(), index.row(), index.internalPointer()), role);

} 

QModelIndex Proxy::index( int row, int column, const QModelIndex &index) const{
  qDebug() << "index:  row " << row << ", col " << column << ", data " << sourceModel()->data(createIndex(row, column, index.internalPointer())).toString(); 
  return sourceModel()->index(column, row, index);
}


QModelIndex Proxy::parent ( const QModelIndex & index ) const{
  return sourceModel()->parent(createIndex(index.column(), index.row(), index.internalPointer()));
}


QVariant Proxy::headerData( int section, Qt::Orientation o, int role) const{
  if (o==Qt::Vertical)  
    return sourceModel()->headerData(section, Qt::Horizontal, role); 
  else
    return "";
}

Qt::ItemFlags Proxy::flags ( const QModelIndex & index ) const{
  return sourceModel()->flags(createIndex(index.column(), index.row(), index.internalPointer()));
}

QModelIndex Proxy::mapToSource ( const QModelIndex & i ) const{
  return createIndex(i.column(),i.row(),i.internalPointer());
};
QModelIndex Proxy::mapFromSource ( const QModelIndex & i ) const{
  return createIndex(i.column(),i.row(),i.internalPointer());
}


#endif
