//=====================================================================
// Project:      unREAL
// File Name:    diagramexplorermodel.cpp
// Description:  Proxy model for Diagram Explorer
//
// Created:      16-Jan-07
// Revision:     01-Feb-07
//
// Author:       Timofey A. Bryksin (sly@tercom.ru)
//===================================================================== 

#include "diagramexplorermodel.h"
#include "dbg.h"

DiagramExplorerModel:: DiagramExplorerModel(QSqlDatabase &_db, QObject *parent) : QAbstractItemModel(parent) {
//dbg;

    db = _db;
    diagrams = new QMap<QString, QSqlQuery>;
    rootItem = new TreeItem("diagram", "diagram", "", diagrams, 0);   
    rescan();  
  
    curID = 666;
    elemID = 0;
}  

void DiagramExplorerModel::rescan(){
dbg;

    TreeItem *table, *value;

    QSqlQuery q, q1,q2;  
    q.prepare("select name from diagram where type='diagrams'");
    diagrams->insert("diagram", q);
  
    q1.exec("select name from diagram where type='diagrams'");
    int nameClmn = q1.record().indexOf("name");
    while(q1.next()){           // fetching diagram names
        QString tableName = q1.value(nameClmn).toString();    
        table = new TreeItem(tableName, "diagram", "diagram", diagrams, rootItem);                  
        rootItem->addChild(table);
        q2.prepare("select name from " + tableName);
        diagrams->insert(tableName, q2);
        q2.exec("select * from " + tableName + " order by id");
        int nameCol = q2.record().indexOf("name");
        int typeCol = q2.record().indexOf("type");  
        while(q2.next()){
            QString valueName = q2.value(nameCol).toString();
            QString typeName  = q2.value(typeCol).toString();
            value = new TreeItem(valueName, typeName, tableName, diagrams, table);
            table->addChild(value);
        }    
    }
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
    q1.exec("update " + item->getType() + " set name='" + value.toString() + "' where name='" + item->getName() + "'");
    item->setData(value.toString());
    
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
 
 if (childItem){
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
  return parentItem->rowCount();
}

void DiagramExplorerModel::updateData(const QModelIndex& index, QVariant value){
//dbg;
  TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
  rootItem->getChild(item->getDiagramName())->getChild(item->getName())->setData(value.toString());
}

QModelIndex DiagramExplorerModel::getIndex(QString id){
//dbg;
    QString diagram = id.section('/',1,1);
    QString name    = id.section('/',2,2);
    TreeItem* item = rootItem->getChild(diagram)->getChild(name);
    return createIndex(item->row(),0,item);
}

void DiagramExplorerModel::createDiagram(QString &dname){
//dbg;
    QSqlQuery q;    
    QString tmp = "insert into diagram (name, type) values ('" + dname + "', 'diagrams')";
    q.exec(tmp);//.arg(curID));
    //curID++;
    tmp = "create table %1 (id integer primary key auto_increment, name varchar(20), type varchar(20))";
    q.exec(tmp.arg(dname));
    
    rootItem = new TreeItem("diagram", "diagram", "", diagrams, 0);   
    rescan();
}

void DiagramExplorerModel::createElement(QList<QString> values, QString fields){
dbg;
    QSqlQuery q, q1;

    QString tmp = "insert into %1 (name, type) values ('%2', '%3')";
    q.exec(tmp.arg(values.at(0)).arg(values.at(1)).arg(values.at(6)));
    tmp = "insert into %1 (%2) values ('%3', '%4', %5, '%6', '%7', '%8')";
    tmp = tmp.arg(values.at(6)).arg(fields).arg(values.at(1)).arg(values.at(2)).arg(values.at(3)).arg(values.at(4)).arg(values.at(5)).arg(values.at(0));
    q1.exec(tmp);
    
    rootItem = new TreeItem("diagram", "diagram", "", diagrams, 0);   
    rescan(); 
    
    TreeItem *item = rootItem->getChild("req_diagram_");
    for (int i = 0; i < item->childCount(); i++){
     qDebug() << item->getChild(i)->getName() << item->getChild(i)->getType();
     }
    
    emit elemAdded();
}


