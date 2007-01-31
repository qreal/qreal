//=====================================================================
// Project:      unREAL
// File Name:    diagramexplorermodel.cpp
// Description:  Proxy model for Diagram Explorer
//
// Created:      16-January-07
// Revision:      
//
// Author:       Timofey A. Bryksin (sly@tercom.ru)
//===================================================================== 

#include "diagramexplorermodel.h"
//#include "dbg.h"

DiagramExplorerModel:: DiagramExplorerModel(QSqlDatabase &_db, QObject *parent) : QAbstractItemModel(parent) {
//dbg;

  db = _db;
  diagrams = new QMap<QString, QSqlQuery>;
  rootItem = new TreeItem("diagram", "diagram", "", diagrams, 0);   
  
  TreeItem *table, *value;
  
  QSqlQuery q, q1,q2;  
  q.prepare("select name from diagram where type='diagrams'");
  diagrams->insert("diagram", q);
  
  q1.exec("select name from diagram where type='diagrams'");
  //qDebug() << q1.executedQuery();
  int nameClmn = q1.record().indexOf("name");
  while(q1.next()){           // fetching diagram names
    QString tableName = q1.value(nameClmn).toString();    
    table = new TreeItem(tableName, "diagram", "diagram", diagrams, rootItem);                  
    rootItem->addChild(table);
    q2.prepare("select name from " + tableName);
    diagrams->insert(tableName, q2);
    q2.exec("select * from " + tableName);
  //  qDebug() << q2.executedQuery();
    int nameCol = q2.record().indexOf("name");
    int typeCol = q2.record().indexOf("type");  
    while(q2.next()){
       QString valueName = q2.value(nameCol).toString();
       QString typeName  = q2.value(typeCol).toString();
    //   qDebug() << valueName << typeName;
       value = new TreeItem(valueName, typeName, tableName, diagrams, table);
       table->addChild(value);
    }    
  }
 /* 
  qDebug() << "testing... size is " << diagrams->size();
  
  qDebug() << rootItem->rowCount();
  qDebug() << rootItem->getName();
  qDebug() << "  " << rootItem->getChild(0)->getName();
  qDebug() << "    " << rootItem->getChild(0)->getChild(0)->getName();
  qDebug() << "    " << rootItem->getChild(0)->getChild(1)->getName();

  qDebug() << "  " << rootItem->getChild(1)->getName();
  qDebug() << "    " << rootItem->getChild(1)->getChild(0)->getName();
  qDebug() << "    " << rootItem->getChild(1)->getChild(1)->getName();
*/
}  

DiagramExplorerModel::~ DiagramExplorerModel(){
//dbg;    
  delete rootItem;
}

bool  DiagramExplorerModel::setData(const QModelIndex& index, const QVariant &value, int role){
//dbg;
  if (index.isValid() && role == Qt::EditRole) {
    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    emit dataAboutToBeChanged(index, value);
    
    QSqlQuery q1;
    QSqlQuery q2;
    q1.exec("update " +item->getDiagramName()+ " set name='"+ value.toString() + "' where name='" + item->getName() + "'");
    
//    qDebug() << item->parent()->name << " -- " << q1.executedQuery(); 
//    q2.exec("select name from " + item->parent()->name); 
//    while(q2.next()) qDebug() << q2.value(0).toString();

    q1.exec("update " + item->getType() + " set name='" + value.toString() + "' where name='" + item->getName() + "'");
    
 //   qDebug() << item->type << " -- " << q1.executedQuery(); 
//    q2.exec("select name from " + item->type); 
 //   qDebug() << item->type << " -- " << q2.lastQuery() ;
//    while(q2.next()) qDebug() << q2.value(0).toString();

    item->setData(value.toString());
    
    //qDebug() << q.executedQuery(); q.exec("select name from " + item->parent()->name); 
    //while(q.next()) qDebug() << q.value(0).toString();
    
    emit dataChanged(index, index);
    return true;
  }
  return false;
}



bool  DiagramExplorerModel::insertRows(int position, int rows, const QModelIndex &parent){
//dbg;	
	beginInsertRows(QModelIndex(), position, position + rows - 1);
	
	endInsertRows();
	return true;
}

bool  DiagramExplorerModel::removeRows(int position, int rows, const QModelIndex &parent){
//dbg; 
 beginRemoveRows(QModelIndex(), position, position + rows - 1);
 
 endRemoveRows();
 return true;
}

int DiagramExplorerModel::columnCount(const QModelIndex &parent) const{
//dbg;
  if (parent.isValid())
    return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
  else
    return rootItem->columnCount();
}

QVariant DiagramExplorerModel::data(const QModelIndex &index, int role) const{
//dbg;
 if (!index.isValid())
   return QVariant();
 if (role != Qt::DisplayRole)
   return QVariant();
 TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
 //qDebug() << "== " << __FUNCTION__ << ": " << item->data(index.column());  

 return item->data();
}

Qt::ItemFlags DiagramExplorerModel::flags(const QModelIndex &index) const{
//dbg;
  Qt::ItemFlags f = Qt::ItemIsEnabled;
  if (!index.isValid())
    return f;
  TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
  if (!item->isTable())
    f |= Qt::ItemIsEditable;
  return f | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant DiagramExplorerModel::headerData(int , Qt::Orientation , int ) const {
//dbg;                               
// if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
//   return rootItem->data(section);
 //return QVariant();
 return "";
}

QModelIndex DiagramExplorerModel::index(int row, int column, const QModelIndex &parent)
            const{
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

QModelIndex DiagramExplorerModel::parent(const QModelIndex &index) const{
//dbg;
 if (!index.isValid())
   return QModelIndex();
 TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
 TreeItem *parentItem = childItem->parent();
 if (parentItem == rootItem)
   return QModelIndex();
 return createIndex(parentItem->row(), 0, parentItem);
}

int DiagramExplorerModel::rowCount(const QModelIndex &parent) const{
//dbg;
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

void DiagramExplorerModel::updateData(const QModelIndex& index, QVariant value){
//dbg;
  TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
  //qDebug() << item->getDiagramName() << item->getName() << value.toString();
  rootItem->getChild(item->getDiagramName())->getChild(item->getName())->setData(value.toString());
}

QModelIndex DiagramExplorerModel::getIndex(QString id){
//dbg;
    QString diagram = id.section('/',1,1);
    QString name    = id.section('/',2,2);
    TreeItem* item = rootItem->getChild(diagram)->getChild(name);
    return createIndex(item->row(),0,item);
}

