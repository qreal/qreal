//=====================================================================
// Project:      unREAL
// File Name:    propertyeditormodel.cpp
// Description:  Proxy model for Property Editor
//
// Created:      29-January-07
// Revision:      
//
// Author:       Timofey A. Bryksin (sly@tercom.ru)
//===================================================================== 

#include "propertyeditormodel.h"
#include "dbg.h"

PropertyEditorModel:: PropertyEditorModel(QObject *parent) : QSqlTableModel(parent) {
dbg;
}  

PropertyEditorModel::~ PropertyEditorModel(){
dbg;    
}
/*
void PropertyEditorModel::updateData(const QModelIndex& index, QVariant value){
//dbg;
    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    QString tmp = "update %1 set %2=%3 where %4=%5";
    db.exec()
  //qDebug() << item->getDiagramName() << item->getName() << value.toString();
  //rootItem->getChild(item->getDiagramName())->getChild(item->getName())->setData(value.toString());
}
*/
void PropertyEditorModel::setFocus( const QModelIndex &index){
dbg;
    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    setTable(item->getType());
    qDebug() << item->getType() << item->getName();
    QString tmp = "name='" + item->getName() + "'";
    qDebug() << tmp; 
    setFilter(tmp);
    select();
    QSqlRecord r = record(0);
    for (int i=0; i < r.count(); i++){
        setHeaderData(i, Qt::Horizontal, headerData(i, Qt::Horizontal )); 
    }
}

