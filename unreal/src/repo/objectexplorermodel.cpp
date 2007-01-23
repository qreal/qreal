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

#include <QtGui>
#include "objectexplorermodel.h"
#include "dbg.h"


ObjectExplorerModel::ObjectExplorerModel(QSqlDatabase &_db, QObject *parent) : QAbstractItemModel(parent) {
dbg;
  createMaps();
 
  db = _db;
 
  rootItem = new TreeItem("diagram", "diagram", "", objects, 0); 
  rootItem->addChild(new TreeItem("actor", "diagram", "diagram", objects, rootItem)); 
  rootItem->addChild(new TreeItem("usecase", "diagram", "diagram", objects, rootItem)); 
  TreeItem *tmp1 = rootItem->getChild("actor");
  tmp1->addChild(new TreeItem("actor 1", "actor", "awesome_diagram", objects, tmp1));
  tmp1->addChild(new TreeItem("actor 2", "actor", "super_cool_diagram", objects, tmp1));
  tmp1->addChild(new TreeItem("actor 4", "actor", "super_cool_diagram", objects, tmp1));
  TreeItem *tmp2 = rootItem->getChild("usecase");
  tmp2->addChild(new TreeItem("usecase 1", "usecase", "awesome_diagram", objects, tmp2));

}

void ObjectExplorerModel::createMaps(){
//dbg;
  objects = new QMap<QString, QSqlQuery>;

  QSqlQuery o1;
  o1.prepare("select name from actor");
  objects->insert("actor", o1);
   
  QSqlQuery o2;
  o2.prepare("select name from usecase");
  objects->insert("usecase", o2);
  
  QSqlQuery o3;
  o3.prepare("select name from diagram where type='objects'");
  objects->insert("diagram", o3);

  diagrams = new QMap<QString, QSqlQuery>;
  
  QSqlQuery d1;
  d1.prepare("select name from awesome_diagram");
  diagrams->insert("awesome_diagram", d1);
   
  QSqlQuery d2;
  d2.prepare("select name from super_cool_diagram");
  diagrams->insert("super_cool_diagram", d2);

  QSqlQuery d3;
  d3.prepare("select name from diagram where type='diagrams'");
  diagrams->insert("diagram", d3);


}

ObjectExplorerModel::~ObjectExplorerModel(){
dbg;    
  delete rootItem;
}

bool ObjectExplorerModel::setData(const QModelIndex& index, const QVariant &value, int role){
dbg;
  if (index.isValid() && role == Qt::EditRole) {
    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    emit dataAboutToBeChanged(index, value);
    QSqlQuery q;
    q.exec("update " +item->getDiagramName()+ " set name='" + value.toString() + "' where name='" + item->getName() + "'");
    q.exec("update " +item->getType() + " set name='" + value.toString() + "' where name='" + item->getName() + "'");
    item->setData(value.toString());
   // qDebug() << q.executedQuery();
    //q.exec("select name from " + item->parent()->name);
    //while(q.next()) qDebug() << q.value(0).toString();
    emit dataChanged(index, index);
    return true;
  }
  return false;
}


/*
bool ObjectExplorerModel::insertRows(int position, int rows, const QModelIndex &parent){
	
	beginInsertRows(QModelIndex(), position, position + rows - 1);
	
	endInsertRows();
	return true;
}

bool ObjectExplorerModel::removeRows(int position, int rows, const QModelIndex &parent){
 
 beginRemoveRows(QModelIndex(), position, position + rows - 1);
 
 endRemoveRows();
 return true;
}*/

int ObjectExplorerModel::columnCount(const QModelIndex &parent) const{
//dbg;
  if (parent.isValid())
    return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
  else
    return rootItem->columnCount();
}

QVariant ObjectExplorerModel::data(const QModelIndex &index, int role) const{
//dbg;
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
 return "";
}

QModelIndex ObjectExplorerModel::index(int row, int column, const QModelIndex &parent) const{
//dbg;            
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
 if (!index.isValid())
   return QModelIndex();
 TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
 TreeItem *parentItem = childItem->parent();
 if (parentItem == rootItem)
   return QModelIndex();
 return createIndex(parentItem->row(), 0, parentItem);
}

int ObjectExplorerModel::rowCount(const QModelIndex &parent) const{
//dbg;
 TreeItem *parentItem;
 if (!parent.isValid()){
  parentItem = rootItem;
}  
 else{
  parentItem = static_cast<TreeItem*>(parent.internalPointer());

 }  
 //qDebug() << "==" << __FUNCTION__ << ": " << parentItem->rowCount() << ", name = " << parentItem->name;  
 return parentItem->rowCount();
}

void ObjectExplorerModel::updateData(const QModelIndex &index, QVariant value){
//dbg;
  TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
  rootItem->getChild(item->getType())->getChild(item->getName())->setData(value.toString());
}
