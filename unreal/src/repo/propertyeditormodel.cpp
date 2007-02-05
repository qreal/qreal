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
//#include "dbg.h"

PropertyEditorModel:: PropertyEditorModel(QObject *parent) : QSqlTableModel(parent) {
//dbg;
  
}  

PropertyEditorModel::~ PropertyEditorModel(){
//dbg;    
  
}

void PropertyEditorModel::updateData(const QModelIndex& index, QVariant value){
//dbg;
//  TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
  //qDebug() << item->getDiagramName() << item->getName() << value.toString();
  //rootItem->getChild(item->getDiagramName())->getChild(item->getName())->setData(value.toString());
}

void PropertyEditorModel::setFocus( const QModelIndex &index){
//dbg;
    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
/*    if (item->getType() == "diagrams"){
        reset();
        return;
    }  */
//    qDebug() << item->getType() << item->getName();
    setTable(item->getType());
    QString tmp = "name='" + item->getName() + "'";
    setFilter(tmp);
    select();
//    qDebug() << tableName();
}

