#include <QSortFilterProxyModel>
#include "treeitem.h"


class Proxy : public QSortFilterProxyModel{

  Q_OBJECT

public:
  Proxy(){};
  ~Proxy(){};
  
  int columnCount( const QModelIndex& ) const;
  int rowCount( const QModelIndex& ) const;
  QVariant data(const QModelIndex&, int ) const;
  QModelIndex index( int row, int column, const QModelIndex &parent = QModelIndex()) const;
  QVariant headerData( int section, Qt::Orientation, int role = Qt::DisplayRole) const;
  Qt::ItemFlags flags ( const QModelIndex & index ) const;
  QModelIndex mapToSource ( const QModelIndex & proxyIndex ) const;
  QModelIndex mapFromSource ( const QModelIndex & proxyIndex ) const;
  QModelIndex parent ( const QModelIndex & index ) const;
};
