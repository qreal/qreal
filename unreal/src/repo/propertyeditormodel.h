//=====================================================================
// Project:      unREAL
// File Name:    propertyeditormodel.h
// Description:  Proxy model for Property Editor
//
// Created:      29-Jan-07
// Revision:     23-Feb-07 
//
// Author:       Timofey A. Bryksin (sly@tercom.ru)
//===================================================================== 

#ifndef PROPERTYEDITORMODEL_H
#define PROPERTYEDITORMODEL_H

#include <QSqlTableModel>
#include "treeitem.h"

class TreeItem;

// proxy model for property editor

class PropertyEditorModel : public QSqlTableModel
{
    Q_OBJECT

public:
    PropertyEditorModel(QObject *parent = 0);
    ~PropertyEditorModel();

//    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);            
           
signals:    
    void dataAboutToBeChanged( const QModelIndex&, QVariant );

public slots:
//    void updateData( const QModelIndex&, QVariant );
    void setFocus( const QModelIndex& );

private:                  
    //TreeItem *rootItem;
    //QSqlDatabase db;
    //QMap<QString, QSqlQuery> *objects;
    //QMap<QString, QSqlQuery> *diagrams;
 
};


#endif
