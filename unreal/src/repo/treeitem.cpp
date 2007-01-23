//=====================================================================
// Project:      unREAL 
// File Name:    treeitem.cpp
// Description:  Tree item, provides hierarchical structures for 
//               relational tables 
//
// Created:      16-January-07
// Revision:      
//
// Author:       Timofey A. Bryksin (sly@tercom.ru)
//===================================================================== 

#include <QStringList>
#include <QSqlRecord>
#include <QSqlDriver>
#include <QtGui>
          
#include "treeitem.h"

TreeItem::TreeItem(QString str, QString t, QString d, QMap<QString, QSqlQuery> *map, TreeItem *parent){

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
  if(!tables->contains(name))
    return 0;
  
  if(!(tables->value(name).isActive()))
    (*tables)[name].exec();

  //qDebug() << "name: " << name;
  //qDebug() << "isActive: " << (*tables)[name].isActive();
  bool a = tables->value(name).driver()->hasFeature(QSqlDriver::QuerySize);
  //qDebug() << "can fetch query size: " << a;
  //qDebug() << "size: " << (*tables)[name].size();
  //while((*tables)[name].next()) qDebug() << "  " << (*tables)[name].value(0).toString();
  
  if (a)
    return tables->value(name).size();
  else{ // really horrible way, but nothing else to do if size() is not supported
    (*tables)[name].last();
    //qDebug() << "rows: " << (*tables)[name].at() + 1;
    return tables->value(name).at() + 1;
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
