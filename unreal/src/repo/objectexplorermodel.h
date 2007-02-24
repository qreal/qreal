//=====================================================================
// Project:      unREAL 
// File Name:    objectexplorermodel.h
// Description:  Proxy model for Object Explorer
//
// Created:      16-Jan-07
// Revision:     23-Feb-07 
//
// Author:       Timofey A. Bryksin (sly@tercom.ru)
//===================================================================== 

#ifndef OBJECTEXPLORERMODEL_H
#define OBJECTEXPLORERMODEL_H

#include <QtGui>
#include <QAbstractItemModel>

#include "treeitem.h"

class TreeItem;

class ObjectExplorerModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    ObjectExplorerModel(QSqlDatabase &, QObject *parent = 0);
    ~ObjectExplorerModel();

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole); 
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
 
    void preInsertRows( int , QString );
    int elementExists( QString, QString, QString );    
       
signals:
    void nameAboutToBeChanged( QStringList );

public slots:
    void updateData(const QModelIndex &, QVariant);
    void addElem( QStringList );
    void removeElem( QStringList );
    void nameChanged( QStringList );
    
private:          
    void rescan(); // DO NOT USE IT OUTSIDE THE CONSTRUCTOR!! YOU GONNA BURN IN HELL FOR THAT!
    TreeItem *rootItem;
    QSqlDatabase db;
    QMap<QString, QString> *objects;
    QMap<QString, QString> *diagrams;
};


#endif

