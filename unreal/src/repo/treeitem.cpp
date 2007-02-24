//=====================================================================
// Project:      unREAL 
// File Name:    treeitem.cpp
// Description:  Tree item, provides hierarchical structures for 
//               relational tables 
//
// Created:      16-Jan-07
// Revision:     20-Feb-07 
//
// Author:       Timofey A. Bryksin (sly@tercom.ru)
//===================================================================== 

#include <QStringList>
#include <QSqlRecord>
#include <QSqlDriver>
#include <QtGui>
#include "dbg.h"
          
#include "treeitem.h"

TreeItem::TreeItem(QString str, QString t, QString d, QMap<QString, QString> *map, TreeItem *parent, QSqlDatabase _db){

    db = _db;
    parentItem = parent;
    name = str;   
    tables = map;
    type = t;
    diagramName = d;
}

TreeItem::~TreeItem(){

  qDeleteAll(childItems);
}

int TreeItem::rowCount() const{
dbg; 
    if(!tables->contains(name))
        return 0;
  
    QSqlQuery qq = db.exec((*tables)[name]);
    bool a = db.driver()->hasFeature(QSqlDriver::QuerySize);
    if (a){
        return qq.size();
    }    
    else{ // really horrible way, but nothing else to do if size() is not supported
        qq.last();
        return qq.at() + 1;
    }  
}

void TreeItem::addChild(TreeItem* item){
  childItems.append(item);   
}

TreeItem* TreeItem::getChild(QString str){
  
    int i=0;
    while (i<childItems.size() && childItems.at(i)->name != str)
        i++;
    if(i == childItems.size())
        return 0;
    else 
        return childItems.value(i);
}

TreeItem* TreeItem::getChild( int pos ){
  if (pos <= childItems.size() && pos >= 0)
    return childItems.value(pos);
  else 
    return 0;
}

int TreeItem::row(){
  if (parentItem)
    return parentItem->childItems.indexOf(const_cast<TreeItem*>(this));
  return 0;  
}

bool TreeItem::isTable(){
  return tables->contains(name);
}

void TreeItem::removeChild( QString name ){
    int i=0;
    while (i < childItems.size() && childItems.at(i)->name != name)
        i++;
    if (childItems.at(i)->name == name){
        delete childItems.at(i);
        childItems.removeAt(i);
    }
}

void TreeItem::setEnds( QString b, QString e){
    beginsWith = b;
    endsWith   = e;
}
