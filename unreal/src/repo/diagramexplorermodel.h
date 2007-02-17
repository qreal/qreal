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
    void createDiagram(QString& );
    void removeDiagram(QString& );
    void createElement(QList<QString> values, QString fields);
    void removeElement(QString name, QString diagram);
    QModelIndex getDiagramIndex( QString& );
    QModelIndex getBeginning( QModelIndex& );
    QModelIndex getEnding( QModelIndex& );

signals:    
    void dataAboutToBeChanged(const QModelIndex&, QVariant);
    void elemAdded();

public slots:
    void updateData(const QModelIndex&, QVariant);

private:              
    //BaseModel *model;
    int curID;
    int elemID;
    TreeItem *rootItem;
    QSqlDatabase db;
    QMap<QString, QString> *objects;
    QMap<QString, QString> *diagrams;
 
    void rescan(); // DO NOT USE IT OUTSIDE THE CONSTRUCTOR!! YOU GONNA BURN IN HELL OF THAT!!
    
};


#endif

