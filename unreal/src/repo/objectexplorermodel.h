//=====================================================================
// Project:      unREAL 
// File Name:    objectexplorermodel.h
// Description:  Proxy model for Object Explorer
//
// Created:      16-January-07
// Revision:      
//
// Author:       Timofey A. Bryksin (sly@tercom.ru)
//===================================================================== 

#ifndef OBJECTEXPLORERMODEL_H
#define OBJECTEXPLORERMODEL_H

#include <QAbstractItemModel>
#include <QVariant>

#include "treeitem.h"

class TreeItem;

class ObjectExplorerModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    ObjectExplorerModel(QSqlDatabase &, QObject *parent = 0);
    ~ObjectExplorerModel();

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole); 
//    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex());
//    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
 
    void createMaps();

signals:
    void dataAboutToBeChanged(const QModelIndex &, QVariant);

public slots:
    void updateData(const QModelIndex &, QVariant);
 
private:              
    TreeItem *rootItem;
    QSqlDatabase db;
    QMap<QString, QSqlQuery> *objects;
    QMap<QString, QSqlQuery> *diagrams;
};


#endif

