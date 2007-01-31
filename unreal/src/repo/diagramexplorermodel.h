//=====================================================================
// Project:      unREAL
// File Name:    diagramexplorermodel.h
// Description:  Proxy model for Diagram Explorer
//
// Created:      16-January-07
// Revision:      
//
// Author:       Timofey A. Bryksin (sly@tercom.ru)
//===================================================================== 

#ifndef DIAGRAMEXPLORERMODEL_H
#define DIAGRAMEXPLORERMODEL_H

#include <QAbstractItemModel>
#include "treeitem.h"

class TreeItem;

// proxy model for diagram explorer

class DiagramExplorerModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    DiagramExplorerModel(QSqlDatabase &, QObject *parent = 0);
    ~DiagramExplorerModel();

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole); 
    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
 
    void prepareInsertion( int, int, QModelIndex, QString, QString, QString, QString );
    QModelIndex getIndex(QString );

signals:    
    void dataAboutToBeChanged(const QModelIndex&, QVariant);

public slots:
    void updateData(const QModelIndex&, QVariant);

private:              
    //BaseModel *model;
    TreeItem *rootItem;
    QSqlDatabase db;
    QMap<QString, QSqlQuery> *objects;
    QMap<QString, QSqlQuery> *diagrams;
 
};


#endif

