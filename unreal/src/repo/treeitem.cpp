//=====================================================================
// Project:      unREAL 
// File Name:    treeitem.cpp
// Description:  Tree item, provides hierarchical structures for 
//               relational tables 
//
// Created:      16-Jan-07
// Revision:     01-Feb-07 
//
// Author:       Timofey A. Bryksin (sly@tercom.ru)
//===================================================================== 

#include <QStringList>
#include <QSqlRecord>
#include <QSqlDriver>
#include <QtGui>
          
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
//dbg; 
    //QSqlDatabase d = QSqlDatabase::database();
    //QSqlQuery aaa = db.exec("select name from diagram where type='diagrams'");
    //qDebug() << "TREEITEM: " << aaa.size();

    if(!tables->contains(name))
        return 0;
  
    //if(!(tables->value(name).isActive()))
    QSqlQuery qq = db.exec((*tables)[name]);

    //qDebug() << (*tables)[name].executedQuery();  
   // qDebug() << "name: " << name;
    bool a = db.driver()->hasFeature(QSqlDriver::QuerySize);
    //qDebug() << "can fetch query size: " << a;
//    qDebug() << "size: " << (*tables)[name].size();
    //while(qq.next()) qDebug() << "  " << qq.value(0).toString();
  
    if (a){
      //  qDebug() << "size is " << qq.size(); 
        return qq.size();
    }    
    else{ // really horrible way, but nothing else to do if size() is not supported
        qq.last();
        //qDebug() << "rows: " << qq.at() + 1;
        return qq.at() + 1;
    }  
}

void TreeItem::addChild(TreeItem* item){
  childItems.append(item);   
}

TreeItem* TreeItem::getChild(QString str){
  
  int i=0;
  while (childItems.at(i)->name != str && i<=childItems.size())
    i++;
  if(childItems.at(i)->name != str)
    return 0;
  else 
    return childItems.value(i);
}

TreeItem* TreeItem::getChild( int pos ){
  //qDebug() << "row requested: " << pos << ", rowCount: " << childItems.size();
  if (pos <= childItems.size() && pos >= 0)
    return childItems.value(pos);
  else 
    return 0;
}

void TreeItem::setData( QString str ){
  name = str;
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
