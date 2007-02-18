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
dbg;
 
    db = _db;
    objects = new QMap<QString, QString>;
    rescan(); 
}

void ObjectExplorerModel::rescan(){
    dbg;
    objects = new QMap<QString, QString>; 
    rootItem = new TreeItem("diagram", "diagram", "", objects, 0, db);   
  
    TreeItem *table, *val;
    QString tmp;
  
    QSqlQuery q1,q2;  
    tmp = "select name from diagram where type='objects'";
    objects->insert("diagram", tmp);
  
    q1 = db.exec(tmp);

    int nameClmn = q1.record().indexOf("name");
    while(q1.next()){           // fetching diagram names
        QString tableName = q1.value(nameClmn).toString();    
        table = new TreeItem(tableName, "diagram", "diagram", objects, rootItem, db);                  
        rootItem->addChild(table);
        tmp = "select name from " + tableName;
        objects->insert(tableName, tmp);
        q2 = db.exec(tmp);
        int nameCol = q2.record().indexOf("name");
        int diagramCol = q2.record().indexOf("diagram");
        while(q2.next()){
            QString valueName = q2.value(nameCol).toString();
            QString diagramName = q2.value(diagramCol).toString();
            val = new TreeItem(valueName, tableName, diagramName, objects, table, db);
            table->addChild(val);
        }    
    }
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
        db.exec("update " +item->getDiagramName()+ " set name='" + value.toString() + "' where name='" + item->getName() + "'");
        db.exec("update " +item->getType() + " set name='" + value.toString() + "' where name='" + item->getName() + "'");
        item->setData(value.toString());
    
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

void ObjectExplorerModel::preInsertRows(int rows, QString type){
dbg;  
    insertRows(0, rows, createIndex(0,0,(void*)rootItem->getChild(type)));
}

int ObjectExplorerModel::columnCount(const QModelIndex &parent) const{
dbg;
    if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

QVariant ObjectExplorerModel::data(const QModelIndex &index, int role) const{
dbg;
    if (!index.isValid())
        return QVariant();
    if (role != Qt::DisplayRole)
        return QVariant();
    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

    return item->data();
}

Qt::ItemFlags ObjectExplorerModel::flags(const QModelIndex &index) const{
dbg;
    Qt::ItemFlags f = Qt::ItemIsEnabled;
    if (!index.isValid())
        return f;
    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    if (!item->isTable())
        f |= Qt::ItemIsEditable;
    return f | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant ObjectExplorerModel::headerData(int , Qt::Orientation , int ) const {
dbg;                               
    return "";
}

QModelIndex ObjectExplorerModel::index(int row, int column, const QModelIndex &parent) const{
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

QModelIndex ObjectExplorerModel::parent(const QModelIndex &index) const{
dbg;
    if (!index.isValid())
        return QModelIndex();
    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parent();
    if (parentItem == rootItem || !parentItem)
        return QModelIndex();
    
    QModelIndex ind = createIndex(parentItem->row(), 0, parentItem);
    return ind;
}

int ObjectExplorerModel::rowCount(const QModelIndex &parent) const{
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

void ObjectExplorerModel::updateData(const QModelIndex &index, QVariant value){
dbg;
    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    rootItem->getChild(item->getType())->getChild(item->getName())->setData(value.toString());
}

void ObjectExplorerModel::addElem( QStringList vals ){
dbg;
    beginInsertRows(QModelIndex(), rowCount(), rowCount());

    QString name    = vals.at(1);
    QString type    = vals.at(2);
    QString diagram = vals.at(0);

    TreeItem *par   = rootItem->getChild(type); 
    TreeItem *child = new TreeItem(name, type, diagram, objects, par, db);
    par->addChild(child);

    endInsertRows();
    
}

void ObjectExplorerModel::removeElem( QStringList vals ){
dbg;
    
    QString name    = vals.at(0);
    QString type    = vals.at(2);
    QString diagram = vals.at(1);
    
    int pos = rootItem->getChild(type)->getChild(name)->row(); 
    beginRemoveRows(QModelIndex(), pos, pos);

    rootItem->getChild(type)->removeChild(name);

    endInsertRows();
}
