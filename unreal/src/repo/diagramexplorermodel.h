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

#include <QtGui>
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
    bool insertRows(int position, int rows, QString fields, QStringList vals, const QModelIndex &index = QModelIndex());
    bool removeRows(int position, int rows, bool esElem, QStringList vals, const QModelIndex &index = QModelIndex());
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
 
    void insert( bool isElement, QString, QStringList );
    void remove( bool isElement, QStringList );
    QModelIndex getDiagramIndex( QString );
    QModelIndex getBeginning( QModelIndex& );
    QModelIndex getEnding( QModelIndex& );
    QStringList getDiagramsList() const { return diagramsList; }
    int elementExists( QString, QString, QString );    
    int diagramExists( QString );    
    
signals:    
    void nameAboutToBeChanged( QStringList );
    void elemAdded( QStringList );
    void elemRemoved( QStringList );
    void diagramNameChanged();

public slots:
    void nameChanged( QStringList );

private:              
    //BaseModel *model;
    int curID;
    int maxID;
    TreeItem *rootItem;
    QSqlDatabase db;
    QStringList diagramsList;
    QMap<QString, QString> *objects;
    QMap<QString, QString> *diagrams;
    QMap<int, TreeItem*> *elements;
 
    void rescan(); // DO NOT USE IT OUTSIDE THE CONSTRUCTOR!! YOU GONNA BURN IN HELL FOR THAT!!
    void createDiagramScriptsExec( QStringList );
    void removeDiagramScriptsExec( const QString& );
    void createElementScriptsExec( QStringList , QString );
    void removeElementScriptsExec( QStringList );
    QModelIndex getIndex( QString );
    int getID();
    int getNextID();
    
};


#endif

