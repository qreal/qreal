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

//#define _LONG_DEBUG
#include "diagramexplorermodel.h"
#include "dbg.h"


DiagramExplorerModel:: DiagramExplorerModel(QSqlDatabase &_db, QObject *parent) : QAbstractItemModel(parent) {
dbg;

    db = _db;
    diagrams = new QMap<QString, QString>;
    rootItem = new TreeItem("diagram", "diagram", "", diagrams, 0, db);   
    diagramsList.clear();
    rescan();  
    
  
    curID = 666;
    elemID = 0;
}  

void DiagramExplorerModel::rescan(){
dbg;

    TreeItem *table, *value;
    QString tmp;

    QSqlQuery q1,q2,q3;  
    tmp = "select name from diagram where type='diagrams'";
    diagrams->insert("diagram", tmp);
  
    q1 = db.exec(tmp);
    int nameClmn = q1.record().indexOf("name");
    while(q1.next()){           // fetching diagram names
        QString tableName = q1.value(nameClmn).toString();    
       // qDebug() << tableName;
        table = new TreeItem(tableName, "diagram", "diagram", diagrams, rootItem, db);                 
        diagramsList << tableName;
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
                q2 = db.exec("select * from eP2N where name='" + valueName + "'");
                if (!q2.next()){
                    qDebug() << "there's no such link in the db, sorry...";
                    return;
                }    
                int fromPos = q2.record().indexOf("beginsWith");
                int toPos   = q2.record().indexOf("endsWith");
                QString beginning = q2.value(fromPos).toString();
                QString ending    = q2.value(toPos).toString();
                value->setEnds(beginning, ending);
            }
            table->addChild(value);
        }    
    }
}

DiagramExplorerModel::~ DiagramExplorerModel(){
dbg;    
  delete rootItem;
}

bool  DiagramExplorerModel::setData(const QModelIndex& index, const QVariant &value, int role){
dbg;
  if (index.isValid() && role == Qt::EditRole) {
    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    emit dataAboutToBeChanged(index, value);
    
    db.exec("update " +item->getDiagramName()+ " set name='"+ value.toString() + "' where name='" + item->getName() + "'");
    db.exec("update " + item->getType() + " set name='" + value.toString() + "' where name='" + item->getName() + "'");
    item->setData(value.toString());
    
    emit dataChanged(index, index);
    return true;
  }
  return false;
}
int DiagramExplorerModel::columnCount(const QModelIndex &parent) const{
dbg;
  if (parent.isValid())
    return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
  else
    return rootItem->columnCount();
}

QVariant DiagramExplorerModel::data(const QModelIndex &index, int role) const{
dbg;
 if (!index.isValid())
   return QVariant();
 if (role != Qt::DisplayRole)
   return QVariant();
 TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

 return item->data();
}

Qt::ItemFlags DiagramExplorerModel::flags(const QModelIndex &index) const{
dbg;
  Qt::ItemFlags f = Qt::ItemIsEnabled;
  if (!index.isValid())
    return f;
  TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
  if (!item->isTable())
    f |= Qt::ItemIsEditable;
  return f | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant DiagramExplorerModel::headerData(int , Qt::Orientation , int ) const {
dbg;                               
 return "";
}

QModelIndex DiagramExplorerModel::index(int row, int column, const QModelIndex &parent)
            const{
dbg;            
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
dbg;
 if (!index.isValid())
   return QModelIndex();
 TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
 TreeItem *parentItem = childItem->parent();
 if (parentItem == rootItem)
   return QModelIndex();
 return createIndex(parentItem->row(), 0, parentItem);
}

int DiagramExplorerModel::rowCount(const QModelIndex &parent) const{
dbg;
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
dbg;
  TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
  rootItem->getChild(item->getDiagramName())->getChild(item->getName())->setData(value.toString());
}

void DiagramExplorerModel::createDiagramScriptsExec(const QString &dname){
dbg;
    QString tmp = "insert into diagram (name, type) values ('" + dname + "', 'diagrams')";
    db.exec(tmp);
    
    tmp = "create table " + dname + " (id integer primary key auto_increment, name varchar(20), type varchar(20))";
    db.exec(tmp);
}

void DiagramExplorerModel::removeDiagramScriptsExec(const QString &name){
dbg;
    QString tmp;
    QStringList l;

    TreeItem *d = rootItem->getChild(name);
    int cnt = d->childCount();
    
    for (int i=0; i<cnt; i++){  // mutilate children! ARRRRRRRRRRRRRRRGGGGGGGHHH!!
        l.clear();
        l << d->getChild(0)->getName() << name;
        remove(true, l);
    }    
    
    tmp = "drop table %1";
    db.exec(tmp.arg(name));
    tmp = "delete from diagram where name='%1'";
    db.exec(tmp.arg(name));
}

void DiagramExplorerModel::createElementScriptsExec(QStringList values, QString fields){
dbg;
    QString diagram = values.at(0);
    QString name    = values.at(1);
    QString type    = values.at(2);
    
    QString tmp = "insert into %1 (name, type) values ('%2', '%3')";
    tmp = tmp.arg(diagram).arg(name).arg(type) ;
    db.exec(tmp);

    if (type != "eP2N"){
        QString desc    = values.at(3);
        QString prio    = values.at(4);
        QString source  = values.at(5);
        QString stat    = values.at(6);
    
        tmp = "insert into %1 (%2) values ('%3', '%4', %5, '%6', '%7', '%8')";
        tmp = tmp.arg(type).arg(fields).arg(name).arg(desc).arg(prio).arg(source).arg(stat).arg(diagram);
        db.exec(tmp);
    }
    else{
        QString from = values.at(3);
        QString to   = values.at(4);
        tmp = "insert into %1 (%2) values ('%3', '%4', '%5', '%6')";
        tmp = tmp.arg(type).arg(fields).arg(name).arg(from).arg(to).arg(diagram);
    }
}

void DiagramExplorerModel::removeElementScriptsExec( QStringList vals ){
dbg;   
    QString name    = vals.at(0);
    QString diagram = vals.at(1);
    QString type = rootItem->getChild(diagram)->getChild(name)->getType();
    QString tmp = "delete from %1 where name='%2'";
    db.exec(tmp.arg(diagram).arg(name));
    db.exec(tmp.arg(type).arg(name));
}    

QModelIndex DiagramExplorerModel::getDiagramIndex( QString name ){
dbg;
    if (name != "")
        return  createIndex(rootItem->getChild(name)->row(),0,(void*)rootItem->getChild(name));
    else 
        return QModelIndex();
}

QModelIndex DiagramExplorerModel::getIndex(QString id){
dbg;
    QString diagram = id.section('/',0,0);
    QString name    = id.section('/',1,1);
    qDebug() << "getIndex(): " << diagram << name;
    TreeItem* item = rootItem->getChild(diagram)->getChild(name);
    return createIndex(item->row(),0,item);
}


QModelIndex DiagramExplorerModel::getBeginning( QModelIndex& index ){
dbg;
    TreeItem* it = static_cast<TreeItem*>(index.internalPointer());
    qDebug() << "requesting for " << it->getName() << it->getBeginning(); 
    return getIndex(it->getBeginning());
}

QModelIndex DiagramExplorerModel::getEnding( QModelIndex& index ){
dbg;
    TreeItem* it = static_cast<TreeItem*>(index.internalPointer());
    return getIndex(it->getEnding());
}

void DiagramExplorerModel::insert(bool isElement, QString fields, QStringList values){
dbg;
    
    QModelIndex index;
    TreeItem* par; 
    
    if (isElement)
        par = rootItem->getChild(values.at(0));
    else
        par = rootItem;
         
    index = createIndex( par->row(), 0, (void*) par );
    
    if (!insertRows(rowCount(), 1, fields, values, index))
        qDebug() << "cannot create new row"; 

    if (isElement)
        emit elemAdded(values);    
    else{
        diagramsList << values.at(0);
    }    
}

bool  DiagramExplorerModel::insertRows(int position, int rows, QString fields, QStringList vals, const QModelIndex &parent){
dbg;	
    beginInsertRows(QModelIndex(), position, position + rows - 1);
    
    QString name;
    QString type;
    QString diagram;

    if ( fields == "" ){ // creating diagram in the database
        createDiagramScriptsExec(vals.at(0));
        name    = vals.at(0);
        type    = "diagram";
        diagram = "diagram";
    }    
    else{ // creating element in the database
        createElementScriptsExec(vals, fields);
        name    = vals.at(1);
        type    = vals.at(2);
        diagram = vals.at(0);
    }    
    
    TreeItem *par;
    if(parent.isValid())
        par = static_cast<TreeItem*>(parent.internalPointer());
    else 
        par = 0;
      
    TreeItem *child = new TreeItem(name, type, diagram, diagrams, par, db);
    
    if( fields == ""){
        QString tmp = "select * from " + name;
        diagrams->insert(name, tmp);
    }
   
    if (type == "eP2N"){
        QString beginning = vals.at(3);
        QString ending    = vals.at(4);
        child->setEnds(beginning, ending);
    }
    
    if (par)
        par->addChild(child);
    
	endInsertRows();
	return true;
}

void DiagramExplorerModel::remove(bool isElement, QStringList values){
dbg;
    QModelIndex index;
    TreeItem* par; 
    if (isElement){
        par = rootItem->getChild(values.at(1));
        values << par->getChild(values.at(0))->getType();
    }    
    else
        par = rootItem;
    
    index = createIndex( par->row(), 0, (void*) par );
       
    if (!removeRows(par->getChild(values.at(0))->row(), 1, isElement, values, index))
        qDebug() << "cannot remove row"; 

    if (isElement)
        emit elemRemoved(values);    
    else{
        diagramsList.removeAt(diagramsList.indexOf(values.at(0)));
    }    
}


bool  DiagramExplorerModel::removeRows(int position, int rows, bool isElement, QStringList vals, const QModelIndex &parent){
dbg; 
    beginRemoveRows(QModelIndex(), position, position + rows - 1);
    
    if ( !isElement ) // removing diagram from the database
        removeDiagramScriptsExec(vals.at(0));
    else // removing element from the database
        removeElementScriptsExec(vals);
    
    TreeItem *par;
    if(parent.isValid())
        par = static_cast<TreeItem*>(parent.internalPointer());
    else 
        return false;
    
    QString name = vals.at(0);
    par->removeChild(name);
    
    if( !isElement )
        diagrams->remove(name);
    
    endRemoveRows();
    return true;
}
