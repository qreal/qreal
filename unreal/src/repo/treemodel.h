//=====================================================================
// Project:      altREAL
// File Name:    treemodel.h
// Description:  Qt Repository-based Model 
//
// Date:         21-November-06
// Revision:     
//
// Author:       Timofey A. Bryksin (sly@tercom.ru)
//===================================================================== 

#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QFile>           

#include "R_Entity.h"
#include "treemodel.h"

using namespace Interfaces;
using namespace Storage;


class TreeItem;

class TreeModel : public QAbstractItemModel {

    Q_OBJECT
  
public:
    TreeModel(QObject *parent = 0);
    ~TreeModel();

    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole); 
    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());
    R_Entity *root, *ent1, *ent2, *link;
    Repository *repa; 
protected:              
    void parseFile(const QString &fileName);    
    QString filename;
};

#endif

