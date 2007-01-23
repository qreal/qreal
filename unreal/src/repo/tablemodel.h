#ifndef TABLEMODEL_h
#define TABLEMODEL_H

#include <QSqlQueryModel>
#include "treeitem.h"

class TableModel : public QSqlQueryModel{

  Q_OBJECT

public:
  TableModel(){};
  ~TableModel(){};

  Qt::ItemFlags flags(const QModelIndex &index) const;
  bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole); 

public slots:
  void reset(const QModelIndex& );

};  

#endif
