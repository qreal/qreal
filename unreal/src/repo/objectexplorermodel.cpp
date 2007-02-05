//=====================================================================
// Project:      unREAL 
// File Name:    objectexplorermodel.cpp
// Description:  Proxy model for Object Explorer
//
// Created:      16-January-07
// Revision:      
//
// Author:       Timofey A. Bryksin (sly@tercom.ru)
//===================================================================== 

//#include <QtGui>
#include "objectexplorermodel.h"
#include "dbg.h"


ObjectExplorerModel::ObjectExplorerModel(QSqlDatabase &_db, QObject *parent) : QAbstractItemModel(parent) {
//dbg;
 
  //qDebug() << __FUNCTION__;  

  db = _db;
  objects = new QMap<QString, QSqlQuery>;

    rescan(); 
 
}

void ObjectExplorerModel::rescan(){
//dbg;
  //qDebug() << __FUNCTION__;  
    objects = new QMap<QString, QSqlQuery>; 
    rootItem = new TreeItem("diagram", "diagram", "", objects, 0);   
  
    TreeItem *table, *val;
  
    QSqlQuery q, q1,q2;  
    q.prepare("select name from diagram where type='objects'");
    objects->insert("diagram", q);
  
    q1.exec("select name from diagram where type='objects'");
//      qDebug() << q1.executedQuery();
 //   qDebug() << q1.executedQuery(); //q1.exec("select name from diag" + item->parent()->getType());
   // while(q1.next()) qDebug() << q1.value(0).toString();


    int nameClmn = q1.record().indexOf("name");
    while(q1.next()){           // fetching diagram names
        QString tableName = q1.value(nameClmn).toString();    
        table = new TreeItem(tableName, "diagram", "diagram", objects, rootItem);                  
        rootItem->addChild(table);
        q2.prepare("select name from " + tableName);
        objects->insert(tableName, q2);
        q2.exec("select * from " + tableName);
        //    qDebug() << q2.executedQuery();
        int nameCol = q2.record().indexOf("name");
        int diagramCol = q2.record().indexOf("diagram");
        while(q2.next()){
            QString valueName = q2.value(nameCol).toString();
//            qDebug() << "name: " << valueName;
            QString diagramName = q2.value(diagramCol).toString();
//            qDebug() << "diagram:" << diagramName;
            val = new TreeItem(valueName, tableName, diagramName, objects, table);
    //        qDebug() << "created: " << valueName << " " << tableName << " " << diagramName;
            table->addChild(val);
        }    
    }
 
 
 /*
    qDebug() << "testing... size is " << objects->size();
 
    qDebug() << rootItem->rowCount();
    qDebug() << rootItem->getName();
    
    qDebug() << "  " << rootItem->getChild("nFeatured")->getName();
    qDebug() << "  " << rootItem->getChild("nFeatured")->rowCount();
    qDebug() << "  " << rootItem->getChild("nFeatured")->parent()->getName();*/
    //qDebug() << "  " << rootItem->getChild("nFeatured")->rowCount();
    
    //qDebug() << "    " << rootItem->getChild("nFeatured")->getChild(0)->getName();
//    qDebug() << "    " << rootItem->getChild("req_diagram_1")->getChild(1)->getName();
           
    //qDebug() << "  " << rootItem->getChild(1)->getName();
    //qDebug() << "    " << rootItem->getChild(1)->getChild(0)->getName();
    //qDebug() << "    " << rootItem->getChild(1)->getChild(1)->getName();
                  
  


}


ObjectExplorerModel::~ObjectExplorerModel(){
//dbg;    
  //qDebug() << __FUNCTION__;  
  delete rootItem;
}

bool ObjectExplorerModel::setData(const QModelIndex& index, const QVariant &value, int role){
//dbg;
  //qDebug() << __FUNCTION__;  
  if (index.isValid() && role == Qt::EditRole) {
    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    emit dataAboutToBeChanged(index, value);
    QSqlQuery q;
    q.exec("update " +item->getDiagramName()+ " set name='" + value.toString() + "' where name='" + item->getName() + "'");
    
  //  qDebug() << q.executedQuery(); q.exec("select name from " + item->parent()->getDiagramName());
  //  while(q.next()) qDebug() << q.value(0).toString();
 
    q.exec("update " +item->getType() + " set name='" + value.toString() + "' where name='" + item->getName() + "'");
    item->setData(value.toString());
    
  //  qDebug() << q.executedQuery(); q.exec("select name from " + item->parent()->getType());
  //  while(q.next()) qDebug() << q.value(0).toString();
    
    emit dataChanged(index, index);
    return true;
  }
  return false;
}

void ObjectExplorerModel::preInsertRows(int rows, QString type){
//dbg;  
  //qDebug() << __FUNCTION__;  
  insertRows(0, rows, createIndex(0,0,(void*)rootItem->getChild(type)));
}

bool ObjectExplorerModel::insertRows(int position, int rows, const QModelIndex &parent){
//dbg;	
  //qDebug() << __FUNCTION__;  
	beginInsertRows(QModelIndex(), position, position + rows - 1);

	endInsertRows();
	return true;
}

bool ObjectExplorerModel::removeRows(int position, int rows, const QModelIndex &parent){
//dbg; 
//  qDebug() << __FUNCTION__;  
 beginRemoveRows(QModelIndex(), position, position + rows - 1);
 
 endRemoveRows();
 return true;
}

int ObjectExplorerModel::columnCount(const QModelIndex &parent) const{
//dbg;
//  qDebug() << __FUNCTION__;  
  if (parent.isValid())
    return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
  else
    return rootItem->columnCount();
}

QVariant ObjectExplorerModel::data(const QModelIndex &index, int role) const{
//dbg;
//  qDebug() << __FUNCTION__;  
 if (!index.isValid())
   return QVariant();
 if (role != Qt::DisplayRole)
   return QVariant();
 TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
 //qDebug() << "== " << __FUNCTION__ << ": " << item->data(index.column());  

 return item->data();
}

Qt::ItemFlags ObjectExplorerModel::flags(const QModelIndex &index) const{
//dbg;
//  qDebug() << __FUNCTION__;  
  Qt::ItemFlags f = Qt::ItemIsEnabled;
  if (!index.isValid())
    return f;
  TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
  if (!item->isTable())
    f |= Qt::ItemIsEditable;
  return f | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant ObjectExplorerModel::headerData(int , Qt::Orientation , int ) const {
//dbg;                               
//  qDebug() << __FUNCTION__;  
 return "";
}

QModelIndex ObjectExplorerModel::index(int row, int column, const QModelIndex &parent) const{
//dbg;            
  //qDebug() << __FUNCTION__;  
//  qDebug() << __FUNCTION__;  
 TreeItem *parentItem;
 if (!parent.isValid())
   parentItem = rootItem;
 else
   parentItem = static_cast<TreeItem*>(parent.internalPointer());
 TreeItem *childItem = parentItem->getChild(row);
 
// qDebug() << "index: row " << row;
 if (childItem){
  // qDebug() << "index: row " << row << ", column " << column; 
   return createIndex(row, column, childItem);
 }  
 else
   return QModelIndex();
}

QModelIndex ObjectExplorerModel::parent(const QModelIndex &index) const{
//dbg;
 // qDebug() << __FUNCTION__;  
    if (!index.isValid())
        return QModelIndex();
    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parent();
    if (parentItem == rootItem || !parentItem)
        return QModelIndex();
//    qDebug() << "000" << parentItem->getName(); 
    
    QModelIndex ind = createIndex(parentItem->row(), 0, parentItem);
  //  qDebug() << "123";
    return ind;
}

int ObjectExplorerModel::rowCount(const QModelIndex &parent) const{
//dbg;
  //qDebug() << __FUNCTION__;  
 TreeItem *parentItem;
 if (!parent.isValid()){
  parentItem = rootItem;
}  
 else{
  parentItem = static_cast<TreeItem*>(parent.internalPointer());

 }  
 //qDebug() << "==" << __FUNCTION__ << ": " << parentItem->rowCount() << ", name = " << parentItem->getName();  
 return parentItem->rowCount();
}

void ObjectExplorerModel::updateData(const QModelIndex &index, QVariant value){
//dbg;
//  qDebug() << __FUNCTION__;  
  TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
  rootItem->getChild(item->getType())->getChild(item->getName())->setData(value.toString());
}

void ObjectExplorerModel::doNOTuseIt(){
//dbg;
//  qDebug() << __FUNCTION__;  
//    rootItem = new TreeItem("diagram", "diagram", "", objects, 0);   
    rescan();
}
