//=====================================================================
// Project:      unREAL 
// File Name:    propertyeditormodel.h 
// Description:  Model for Property Editor support
//
// Created:      24-Feb-07
// Revision:     25-Feb-07 
//
// Author:       Timofey A. Bryksin (sly@tercom.ru)
//===================================================================== 

#ifndef PROPERTYEDITORMODEL_H
#define PROPERTYEDITORMODEL_H

#include <QtGui>
#include <QAbstractTableModel>

#include "treeitem.h"
#include "diagramexplorermodel.h"
#include "objectexplorermodel.h"

class TreeItem;

class PropertyEditorModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    PropertyEditorModel(QSqlDatabase&, ObjectExplorerModel*, DiagramExplorerModel*, QObject *parent = 0);
    ~PropertyEditorModel();

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole); 
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    //QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    ///QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    
    int elementExists( QString, QString, QString );    
 
signals:
    void nameChanged( QStringList );

public slots:
    void adjustData( const QModelIndex &);
    void updateName( QStringList );
    
private:          
    QModelIndex current;
    DiagramExplorerModel *dem;
    ObjectExplorerModel *oem;
    TreeItem *rootd;
    TreeItem *rooto;
    void rescan( const QModelIndex& ); // DO NOT USE IT OUTSIDE THE CONSTRUCTOR!! YOU GONNA BURN IN HELL FOR THAT!
    QSqlDatabase db;
    QStringList fields;
    QStringList vals;
    QString diagram;
    QString name;
    QString type;
};


#endif

