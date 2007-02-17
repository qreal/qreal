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

    QSqlQuery aaa; 
    db = _db;
    diagrams = new QMap<QString, QString>;
    rootItem = new TreeItem("diagram", "diagram", "", diagrams, 0, db);   
    rescan();  
  
    curID = 666;
    elemID = 0;
}  

void DiagramExplorerModel::rescan(){
dbg;

    TreeItem *table, *value;
    QString tmp;

    QSqlQuery q, q1,q2,q3;  
    tmp = "select name from diagram where type='diagrams'";
    diagrams->insert("diagram", tmp);
  
    q1 = db.exec(tmp);
    int nameClmn = q1.record().indexOf("name");
    while(q1.next()){           // fetching diagram names
        QString tableName = q1.value(nameClmn).toString();    
       // qDebug() << tableName;
        table = new TreeItem(tableName, "diagram", "diagram", diagrams, rootItem, db);                  
        rootItem->addChild(table);
        tmp = "select * from " + tableName;
        diagrams->insert(tableName, tmp);
        q3 = db.exec(tmp);
        int nameCol = q3.record().indexOf("name");
        int typeCol = q3.record().indexOf("type");  
        while(q3.next()){
            QString valueName = q3.value(nameCol).toString();
            QString typeName  = q3.value(typeCol).toString();
            //qDebug() << valueName << typeName;
            value = new TreeItem(valueName, typeName, tableName, diagrams, table, db);
            if (typeName == "eP2N"){
                QString beginning = q3.value(q3.record().indexOf("beginsWith")).toString();
                QString ending    = q3.value(q3.record().indexOf("endsWith")).toString();
                value->setEnds(beginning, ending);
            }
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
    db.exec("update " +item->getDiagramName()+ " set name='"+ value.toString() + "' where name='" + item->getName() + "'");
    db.exec("update " + item->getType() + " set name='" + value.toString() + "' where name='" + item->getName() + "'");
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

void DiagramExplorerModel::createDiagram(QString &dname){
//dbg;
    QString tmp = "insert into diagram (name, type) values ('" + dname + "', 'diagrams')";
    db.exec(tmp);
    
    tmp = "create table " + dname + " (id integer primary key auto_increment, name varchar(20), type varchar(20))";
    db.exec(tmp);
    
    rootItem = new TreeItem("diagram", "diagram", "", diagrams, 0, db);   
    rescan();
}

void DiagramExplorerModel::removeDiagram(QString &name){
//dbg;
    QSqlQuery q,qq;    
    QString tmp;

    TreeItem *d = rootItem->getChild(name);
    int cnt = d->childCount();
    for (int i=0; i<cnt; i++)  // mutilate children! ARRRRRRRRRRRRRRRGGGGGGGHHH!!
        removeElement(d->getChild(i)->getName(), name); 
    
    
    rootItem->removeChild(name);
    tmp = "drop table %1";
    db.exec(tmp.arg(name));
    tmp = "delete from diagram where name='%1'";
    db.exec(tmp.arg(name));
    
    rootItem = new TreeItem("diagram", "diagram", "", diagrams, 0, db);   
    rescan();

    emit elemAdded();
}

void DiagramExplorerModel::createElement(QList<QString> values, QString fields){
dbg;
    QSqlQuery q, q1;

    QString tmp = "insert into %1 (name, type) values ('%2', '%3')";
    tmp = tmp.arg(values.at(0)).arg(values.at(1)).arg(values.at(6)) ;
   // qDebug() << tmp;
    db.exec(tmp);
    tmp = "insert into %1 (%2) values ('%3', '%4', %5, '%6', '%7', '%8')";
    tmp = tmp.arg(values.at(6)).arg(fields).arg(values.at(1)).arg(values.at(2)).arg(values.at(3)).arg(values.at(4)).arg(values.at(5)).arg(values.at(0));
    //qDebug() << tmp;
    db.exec(tmp);
    
    rootItem = new TreeItem("diagram", "diagram", "", diagrams, 0, db);   
    rescan(); 
    
    TreeItem *item = rootItem->getChild("req_diagram_");
  //  for (int i = 0; i < item->childCount(); i++){
  //   qDebug() << item->getChild(i)->getName() << item->getChild(i)->getType();
  //   }
    
    emit elemAdded();
}

void DiagramExplorerModel::removeElement( QString name, QString diagram ){
//dbg;    
    QString type = rootItem->getChild(diagram)->getChild(name)->getType();
    QSqlQuery q;
    QString tmp = "delete from %1 where name='%2'";
    q = db.exec(tmp.arg(diagram).arg(name));
    q = db.exec(tmp.arg(type).arg(name));
    
    rootItem = new TreeItem("diagram", "diagram", "", diagrams, 0, db);   
    rescan(); 
    
    emit elemAdded();
}    

QModelIndex DiagramExplorerModel::getDiagramIndex( QString& name ){
dbg;
    return createIndex(rootItem->getChild(name)->row(),0,(void*)rootItem->getChild(name));
}

QModelIndex DiagramExplorerModel::getIndex(QString id){
//dbg;
    QString diagram = id.section('/',0,0);
    QString name    = id.section('/',1,1);
    TreeItem* item = rootItem->getChild(diagram)->getChild(name);
    return createIndex(item->row(),0,item);
}


QModelIndex DiagramExplorerModel::getBeginning( QModelIndex& index ){
dbg;
    TreeItem* it = static_cast<TreeItem*>(index.internalPointer());
    return getIndex(it->getBeginning());
}

QModelIndex DiagramExplorerModel::getEnding( QModelIndex& index ){
dbg;
    TreeItem* it = static_cast<TreeItem*>(index.internalPointer());
    return getIndex(it->getEnding());
}
